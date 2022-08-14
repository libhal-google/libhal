#include <boost/ut.hpp>
#include <libhal/counter/util.hpp>
#include <system_error>

#include "../ostreams.hpp"

namespace hal {
boost::ut::suite counter_utility_test = []() {
  using namespace boost::ut;

  static constexpr hertz expected_frequency{ 1'000'000 };

  class dummy_counter : public hal::counter
  {
  public:
    uptime_t get_internal_uptime()
    {
      return { .frequency = m_uptime.frequency, .count = m_uptime.count };
    }

  private:
    result<uptime_t> driver_uptime() noexcept override
    {
      m_uptime.count++;
      return m_uptime;
    }

    uptime_t m_uptime{ .frequency = expected_frequency, .count = 0 };
  };

  // =============== timeout ===============

  "hal::create_timeout(hal::counter, 0ns)"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(0);
    dummy_counter test_counter;
    bool success = false;

    // Exercise
    auto timeout_object = create_timeout(test_counter, expected).value();

    hal::attempt_all(
      [&timeout_object]() -> status { return timeout_object(); },
      [&success](match<std::errc, std::errc::timed_out>) { success = true; },
      []() { expect(false) << "std::errc::timed_out was not thrown!"; });

    // Verify
    expect(that % success) << "std::errc::timed_out handler was not called!";
    // Verify: subtract 2 because 2 invocations are required in order to get
    //         the start uptime and another to check what the latest uptime is.
    expect(that % expected.count() ==
           (test_counter.get_internal_uptime().count - 2));
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  "hal::create_timeout(hal::counter, 50ns)"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(50);
    dummy_counter test_counter;
    bool success = false;

    // Exercise
    auto timeout_object = create_timeout(test_counter, expected).value();

    hal::attempt_all(
      [&timeout_object]() -> status {
        for (std::int64_t i = 0; i < expected.count() - 2; i++) {
          if (!timeout_object()) {
            return hal::new_error();
          }
        }
        return timeout_object();
      },
      [&success](match<std::errc, std::errc::timed_out>) { success = true; },
      []() { expect(false) << "std::errc::timed_out was not thrown!"; });

    // Verify
    expect(that % success) << "std::errc::timed_out handler was not called!";
    expect(that % expected.count() ==
           (test_counter.get_internal_uptime().count));
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  "hal::create_timeout(hal::counter, 1337ns)"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(1337);
    dummy_counter test_counter;
    bool success = false;

    // Exercise
    auto timeout_object = create_timeout(test_counter, expected).value();

    hal::attempt_all(
      [&timeout_object]() -> status {
        for (std::int64_t i = 0; i < expected.count() - 2; i++) {
          if (!timeout_object()) {
            return hal::new_error();
          }
        }
        return timeout_object();
      },
      [&success](match<std::errc, std::errc::timed_out>) { success = true; },
      []() { expect(false) << "std::errc::timed_out was not thrown!"; });

    // Verify
    expect(that % success) << "std::errc::timed_out handler was not called!";
    expect(that % expected.count() ==
           (test_counter.get_internal_uptime().count));
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  "hal::create_timeout(hal::counter, -5ns) returns error"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(-5);
    dummy_counter test_counter;

    // Exercise
    auto result = create_timeout(test_counter, expected);

    // Verify
    expect(!bool{ result });
    expect(that % 0 == test_counter.get_internal_uptime().count);
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  // =============== delay ===============

  "hal::delay(hal::counter, 0ns)"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(0);
    dummy_counter test_counter;

    // Exercise
    auto result = delay(test_counter, expected);

    // Verify
    expect(bool{ result });
    // Verify: subtract 2 because 2 invocations are required in order to get
    //         the start uptime and another to check what the latest uptime is.
    expect(that % expected.count() ==
           (test_counter.get_internal_uptime().count - 2));
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  "hal::delay(hal::counter, 50ns)"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(50);
    dummy_counter test_counter;

    // Exercise
    auto result = delay(test_counter, expected);

    // Verify
    expect(bool{ result });
    expect(that % expected.count() == test_counter.get_internal_uptime().count);
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  "hal::delay(hal::counter, 1337ns)"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(1'337);
    dummy_counter test_counter;

    // Exercise
    auto result = delay(test_counter, expected);

    // Verify
    expect(bool{ result });
    expect(that % expected.count() == test_counter.get_internal_uptime().count);
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  "hal::delay(hal::counter, -5ns) returns error"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(-5);
    dummy_counter test_counter;

    // Exercise
    auto result = delay(test_counter, expected);

    // Verify
    expect(!bool{ result });
    expect(that % 0 == test_counter.get_internal_uptime().count);
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };
};
}  // namespace hal
