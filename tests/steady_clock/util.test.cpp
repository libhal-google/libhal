#include <boost/ut.hpp>
#include <libhal/steady_clock/util.hpp>
#include <system_error>

#include "../ostreams.hpp"

namespace hal {
boost::ut::suite steady_clock_utility_test = []() {
  using namespace boost::ut;

  // Make the frequency equal to inverse of the time duration period, giving you
  // the frequency of each tick of time_duration.
  static constexpr hertz expected_frequency =
    static_cast<hertz>(hal::time_duration::period::den);

  class dummy_steady_clock : public hal::steady_clock
  {
  public:
    std::uint64_t uptime = 0;
    bool will_fail = false;

  private:
    hertz driver_frequency() noexcept override
    {
      return expected_frequency;
    }

    result<std::uint64_t> driver_uptime() noexcept override
    {
      if (will_fail) {
        return new_error();
      }
      return uptime++;
    }
  };

  // =============== timeout ===============

  "hal::create_timeout(hal::steady_clock, 0ns)"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(0);
    dummy_steady_clock test_steady_clock;
    bool success = false;

    // Exercise
    auto timeout_object = create_timeout(test_steady_clock, expected).value();

    hal::attempt_all(
      [&timeout_object]() -> status { return timeout_object(); },
      [&success](match<std::errc, std::errc::timed_out>) { success = true; },
      []() { expect(false) << "std::errc::timed_out was not thrown!"; });

    // Verify
    expect(that % success) << "std::errc::timed_out handler was not called!";
    // Verify: subtract 2 because 2 invocations are required in order to get
    //         the start uptime and another to check what the latest uptime is.
    expect(that % expected.count() == (test_steady_clock.uptime - 2));
    expect(that % expected_frequency == test_steady_clock.frequency());
  };

  "hal::create_timeout(hal::steady_clock, 50ns)"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(50);
    dummy_steady_clock test_steady_clock;
    bool success = false;

    // Exercise
    auto timeout_object = create_timeout(test_steady_clock, expected).value();

    hal::attempt_all(
      [&timeout_object]() -> status {
        // Terminate the loop one iteration before the timeout would occur
        for (std::int64_t i = 0; i < expected.count() - 1; i++) {
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
    // After the last call to uptime() the uptime value is incremented by one
    expect(that % expected.count() == test_steady_clock.uptime - 1);
    expect(that % expected_frequency == test_steady_clock.frequency());
  };

  "hal::create_timeout(hal::steady_clock, 1337ns)"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(10);
    dummy_steady_clock test_steady_clock;
    bool success = false;

    // Exercise
    auto timeout_object = create_timeout(test_steady_clock, expected).value();

    hal::attempt_all(
      [&timeout_object]() -> status {
        for (std::int64_t i = 0; i < expected.count() - 1; i++) {
          // error out if this times out early
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
    expect(that % expected.count() == test_steady_clock.uptime - 1);
    expect(that % expected_frequency == test_steady_clock.frequency());
  };

  "hal::create_timeout(hal::steady_clock, -5ns) returns error"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(-5);
    dummy_steady_clock test_steady_clock;

    // Exercise
    auto result = create_timeout(test_steady_clock, expected);

    // Verify
    expect(bool{ result });
    expect(that % 1 == test_steady_clock.uptime);
    expect(that % expected_frequency == test_steady_clock.frequency());
  };

  "hal::create_timeout(hal::steady_clock, 100ns) but fails"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(100);
    dummy_steady_clock test_steady_clock;
    test_steady_clock.will_fail = true;

    // Exercise
    auto result = create_timeout(test_steady_clock, expected);

    // Verify
    expect(!bool{ result });
    expect(that % 0 == test_steady_clock.uptime);
    expect(that % expected_frequency == test_steady_clock.frequency());
  };

  // =============== delay ===============

  "hal::delay(hal::steady_clock, 0ns)"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(0);
    dummy_steady_clock test_steady_clock;

    // Exercise
    auto result = delay(test_steady_clock, expected);

    // Verify
    expect(bool{ result });
    // Verify: subtract 2 because 2 invocations are required in order to get
    //         the start uptime and another to check what the latest uptime is.
    expect(that % expected.count() == (test_steady_clock.uptime - 2));
    expect(that % expected_frequency == test_steady_clock.frequency());
  };

  "hal::delay(hal::steady_clock, 50ns)"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(50);
    dummy_steady_clock test_steady_clock;

    // Exercise
    auto result = delay(test_steady_clock, expected);

    // Verify
    expect(bool{ result });
    expect(that % expected.count() == test_steady_clock.uptime - 1);
    expect(that % expected_frequency == test_steady_clock.frequency());
  };

  "hal::delay(hal::steady_clock, 1337ns)"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(1'337);
    dummy_steady_clock test_steady_clock;

    // Exercise
    auto result = delay(test_steady_clock, expected);

    // Verify
    expect(bool{ result });
    expect(that % expected.count() == test_steady_clock.uptime - 1);
    expect(that % expected_frequency == test_steady_clock.frequency());
  };

  "hal::delay(hal::steady_clock, -5ns) returns error"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(-5);
    dummy_steady_clock test_steady_clock;

    // Exercise
    auto result = delay(test_steady_clock, expected);

    // Verify
    expect(bool{ result });
    // Verify: Adjust uptime by 2 because at least 2 calls to uptime()
    expect(that % 0 == test_steady_clock.uptime - 2);
    expect(that % expected_frequency == test_steady_clock.frequency());
  };

  "hal::delay(hal::steady_clock, 100ns) but fails"_test = []() {
    // Setup
    static constexpr hal::time_duration expected(100);
    dummy_steady_clock test_steady_clock;
    test_steady_clock.will_fail = true;

    // Exercise
    auto result = delay(test_steady_clock, expected);

    // Verify
    expect(!bool{ result });
    // Verify: Adjust uptime by 2 because at least 2 calls to uptime()
    expect(that % 0 == test_steady_clock.uptime);
    expect(that % expected_frequency == test_steady_clock.frequency());
  };
};
}  // namespace hal
