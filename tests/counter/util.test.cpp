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
    expect(that % expected.count() == test_counter.m_uptime.count - 1);
    expect(that % expected_frequency == test_counter.m_uptime.frequency);
  };

  "embed::delay(embed::counter, 50ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(50);
    dummy_counter test_counter;

    // Exercise
    delay(test_counter, expected);

    // Verify
    expect(that % expected.count() == test_counter.m_uptime.count - 1);
    expect(that % expected_frequency == test_counter.m_uptime.frequency);
  };

  "embed::delay(embed::counter, 1337ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(1337);
    dummy_counter test_counter;

    // Exercise
    delay(test_counter, expected);

    // Verify
    expect(that % expected.count() == test_counter.m_uptime.count - 1);
    expect(that % expected_frequency == test_counter.m_uptime.frequency);
  };

  // ====== TO SLEEP ======

  "Verify that embed::to_delay() returns embed::delay_handler"_test = []() {
    // Setup
    dummy_counter test_counter;

    // Exercise
    [[maybe_unused]] std::function<embed::delay_handler> test_subject =
      to_delay(test_counter);
  };

  "embed::to_delay(embed::counter)(0ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(0);
    dummy_counter test_counter;

    // Exercise
    to_delay(test_counter)(expected);

    // Verify
    expect(that % expected.count() == test_counter.m_uptime.count - 1);
    expect(that % expected_frequency == test_counter.m_uptime.frequency);
  };

  "embed::to_delay(embed::counter)(50ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(50);
    dummy_counter test_counter;

    // Exercise
    to_delay(test_counter)(expected);

    // Verify
    expect(that % expected.count() == test_counter.m_uptime.count - 1);
    expect(that % expected_frequency == test_counter.m_uptime.frequency);
  };

  "embed::to_delay(embed::counter)(1337ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(1337);
    dummy_counter test_counter;

    // Exercise
    to_delay(test_counter)(expected);

    // Verify
    expect(that % expected.count() == test_counter.m_uptime.count - 1);
    expect(that % expected_frequency == test_counter.m_uptime.frequency);
  };

  // ====== TO UPTIME ======

  "Verify that embed::to_uptime() returns embed::uptime_handler"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(0);
    dummy_counter test_counter;

    // Exercise
    [[maybe_unused]] std::function<embed::uptime_handler> test_subject =
      to_uptime(test_counter);
  };

  "embed::to_uptime(embed::counter)(0ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(0);
    dummy_counter test_counter;

    // Exercise
    auto test_subject = to_uptime(test_counter);

    for (int i = 0; i < expected.count() + 1; i++) {
      test_subject();
    }

    // Verify
    expect(that % expected.count() == test_counter.m_uptime.count - 1);
    expect(that % expected_frequency == test_counter.m_uptime.frequency);
  };

  "embed::to_uptime(embed::counter)(50ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(50);
    dummy_counter test_counter;

    // Exercise
    auto test_subject = to_uptime(test_counter);

    for (int i = 0; i < expected.count() + 1; i++) {
      test_subject();
    }

    // Verify
    expect(that % expected.count() == test_counter.m_uptime.count - 1);
    expect(that % expected_frequency == test_counter.m_uptime.frequency);
  };

  "embed::to_uptime(embed::counter)(1337ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(1337);
    dummy_counter test_counter;

    // Exercise
    auto test_subject = to_uptime(test_counter);

    for (int i = 0; i < expected.count() + 1; i++) {
      test_subject();
    }

    // Verify
    expect(that % expected.count() == test_counter.m_uptime.count - 1);
    expect(that % expected_frequency == test_counter.m_uptime.frequency);
  };
};
}  // namespace embed
