#include <boost/ut.hpp>
#include <libembeddedhal/counter/util.hpp>

#include "../ostreams.hpp"

namespace embed {
boost::ut::suite counter_utility_test = []() {
  using namespace boost::ut;

  static constexpr frequency expected_frequency =
    embed::frequency(1'000'000'000);

  class dummy_counter : public embed::counter
  {
  public:
    uptime_t get_internal_uptime()
    {
      // embed::delay needs to call the uptime counter at least once to get
      // the starting position, this removes the inital count that is made to
      // start the embed::delay function.
      static constexpr int initialization_cycle_count = 1;
      return { .frequency = m_uptime.frequency,
               .count = m_uptime.count - initialization_cycle_count };
    }

  private:
    boost::leaf::result<uptime_t> driver_uptime() noexcept override
    {
      m_uptime.count++;
      return m_uptime;
    }

    uptime_t m_uptime{ .frequency = expected_frequency, .count = 0 };
  };

  "embed::delay(embed::counter, 0ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(0);
    dummy_counter test_counter;

    // Exercise
    delay(test_counter, expected);

    // Verify
    expect(that % expected.count() == test_counter.get_internal_uptime().count);
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  "embed::delay(embed::counter, 50ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(50);
    dummy_counter test_counter;

    // Exercise
    delay(test_counter, expected);

    // Verify
    expect(that % expected.count() == test_counter.get_internal_uptime().count);
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  "embed::delay(embed::counter, 1337ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(1337);
    dummy_counter test_counter;

    // Exercise
    delay(test_counter, expected);

    // Verify
    expect(that % expected.count() == test_counter.get_internal_uptime().count);
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  // ====== TO SLEEP ======

  "embed::to_sleep(embed::counter)(0ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(0);
    dummy_counter test_counter;

    // Exercise
    to_sleep(test_counter)(expected);

    // Verify
    expect(that % expected.count() == test_counter.get_internal_uptime().count);
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  "embed::to_sleep(embed::counter)(50ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(50);
    dummy_counter test_counter;

    // Exercise
    to_sleep(test_counter)(expected);

    // Verify
    expect(that % expected.count() == test_counter.get_internal_uptime().count);
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  "embed::to_sleep(embed::counter)(1337ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(1337);
    dummy_counter test_counter;

    // Exercise
    to_sleep(test_counter)(expected);

    // Verify
    expect(that % expected.count() == test_counter.get_internal_uptime().count);
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  // ====== TO UPTIME ======

  "embed::to_uptime(embed::counter)(0ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(0);
    dummy_counter test_counter;
    uptime_counter dummy_uptime_counter(test_counter);

    // Exercise
    auto test_subject = to_uptime(dummy_uptime_counter);

    for (int i = 0; i < expected.count() + 1; i++) {
      test_subject();
    }

    // Verify
    expect(that % expected.count() == test_counter.get_internal_uptime().count);
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  "embed::to_uptime(embed::counter)(50ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(50);
    dummy_counter test_counter;
    uptime_counter dummy_uptime_counter(test_counter);

    // Exercise
    auto test_subject = to_uptime(dummy_uptime_counter);

    for (int i = 0; i < expected.count() + 1; i++) {
      test_subject();
    }

    // Verify
    expect(that % expected.count() == test_counter.get_internal_uptime().count);
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };

  "embed::to_uptime(embed::counter)(1337ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(1337);
    dummy_counter test_counter;
    uptime_counter dummy_uptime_counter(test_counter);

    // Exercise
    auto test_subject = to_uptime(dummy_uptime_counter);

    for (int i = 0; i < expected.count() + 1; i++) {
      test_subject();
    }

    // Verify
    expect(that % expected.count() == test_counter.get_internal_uptime().count);
    expect(that % expected_frequency ==
           test_counter.get_internal_uptime().frequency);
  };
};
}  // namespace embed
