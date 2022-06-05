#include <boost/ut.hpp>
#include <libembeddedhal/timer/util.hpp>

#include "../ostreams.hpp"

namespace embed {
boost::ut::suite timer_utility_test = []() {
  using namespace boost::ut;

  class dummy_timer : public embed::timer
  {
  public:
    boost::leaf::result<void> driver_clear() noexcept override { return {}; }
    boost::leaf::result<bool> driver_is_running() noexcept override
    {
      if (m_count < m_delay) {
        m_count++;
        return true;
      }
      return false;
    }
    boost::leaf::result<void> driver_schedule(
      [[maybe_unused]] std::function<void(void)> p_callback,
      std::chrono::nanoseconds p_delay) noexcept override
    {
      m_delay = p_delay;
      m_count = std::chrono::nanoseconds(0);
      return {};
    }

    std::chrono::nanoseconds m_delay{ 0 };
    std::chrono::nanoseconds m_count{ 0 };
  };

  "embed::delay(embed::timer, 0ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(0);
    dummy_timer test_timer;

    // Exercise
    delay(test_timer, expected);

    // Verify
    expect(that % expected == test_timer.m_count);
    expect(that % expected == test_timer.m_delay);
  };

  "embed::delay(embed::timer, 50ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(50);
    dummy_timer test_timer;

    // Exercise
    delay(test_timer, expected);

    // Verify
    expect(that % expected == test_timer.m_count);
    expect(that % expected == test_timer.m_delay);
  };

  "embed::delay(embed::timer, 1337ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(1337);
    dummy_timer test_timer;

    // Exercise
    delay(test_timer, expected);

    // Verify
    expect(that % expected == test_timer.m_count);
    expect(that % expected == test_timer.m_delay);
  };

  // ====== TO SLEEP ======

  "Verify that embed::to_delay() returns embed::delay_handler"_test = []() {
    // Setup
    dummy_timer test_timer;

    // Exercise
    [[maybe_unused]] std::function<embed::delay_handler> test_subject =
      to_delay(test_timer);
  };

  "embed::to_delay(embed::timer)(0ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(0);
    dummy_timer test_timer;

    // Exercise
    to_delay(test_timer)(expected);

    // Verify
    expect(that % expected == test_timer.m_count);
    expect(that % expected == test_timer.m_delay);
  };

  "embed::to_delay(embed::timer)(50ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(50);
    dummy_timer test_timer;

    // Exercise
    to_delay(test_timer)(expected);

    // Verify
    expect(that % expected == test_timer.m_count);
    expect(that % expected == test_timer.m_delay);
  };

  "embed::to_delay(embed::timer)(1337ns)"_test = []() {
    // Setup
    constexpr std::chrono::nanoseconds expected(1337);
    dummy_timer test_timer;

    // Exercise
    to_delay(test_timer)(expected);

    // Verify
    expect(that % expected == test_timer.m_count);
    expect(that % expected == test_timer.m_delay);
  };
};
}  // namespace embed
