#pragma once

#include "../testing.hpp"
#include "interface.hpp"

namespace hal::mock {
/**
 * @addtogroup timer
 * @{
 */
/**
 * @brief Mock timer implementation for use in unit tests and simulations with
 * spy functions for schedule(), clear(), and is_running()
 *
 */
struct timer : public hal::timer
{
  /**
   * @brief Reset spy information for schedule(), clear(), and is_running()
   *
   */
  void reset()
  {
    spy_schedule.reset();
    spy_cancel.reset();
    spy_is_running.reset();
  }

  /// Spy handler for hal::timer::schedule()
  spy_handler<std::function<void(void)>, std::chrono::nanoseconds> spy_schedule;
  /// Spy handler for hal::timer::is_running()
  spy_handler<bool> spy_is_running;
  /// Spy handler for hal::timer::clear()
  spy_handler<bool> spy_cancel;

private:
  status driver_schedule(std::function<void(void)> p_callback,
                         std::chrono::nanoseconds p_delay) noexcept override
  {
    m_is_running = true;
    return spy_schedule.record(p_callback, p_delay);
  };
  result<bool> driver_is_running() noexcept override
  {
    auto result = spy_is_running.record(true);
    if (!result) {
      return result.error();
    }
    return m_is_running;
  }
  status driver_cancel() noexcept override
  {
    m_is_running = false;
    return spy_cancel.record(true);
  }
  bool m_is_running = false;
};
/** @} */
}  // namespace hal::mock
