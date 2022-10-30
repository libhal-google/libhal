#pragma once

#include <chrono>
#include <functional>

#include "../error.hpp"
#include "../units.hpp"

namespace hal {
/**
 * @addtogroup timer
 * Available timer APIs
 * @{
 */
/**
 * @brief Timer hardware abstraction interface.
 *
 * Use this interface for devices and peripherals that have timer like
 * capabilities, such that, when a timer's time has expired, an event,
 * interrupt, or signal is generated.
 *
 * Timer drivers tick period must be an integer multiple of 1 nanosecond,
 * meaning that the only tick period allowed are 1ns, 2ns, up to the maximum
 * holdable in a std::chrono::nanosecond type. sub-nanosecond tick periods are
 * not allowed.
 *
 */
class timer
{
public:
  /**
   * @brief Error type indicating that the desired time delay is not achievable
   * with this timer.
   *
   * This occurs if the time delay is too large based on the tick period of the
   * timer.
   */
  struct out_of_bounds
  {
    /// The tick period
    std::chrono::nanoseconds tick_period;
    /// The maximum possible delay allowed
    std::chrono::nanoseconds maximum;
  };

  /**
   * @brief Determine if the timer is currently running
   *
   * @return result<bool> - true if timer is currently running
   * @return result<bool> - driver specific error, if any.
   */
  [[nodiscard]] result<bool> is_running() noexcept
  {
    return driver_is_running();
  }

  /**
   * @brief Stops a scheduled event from happening.
   *
   * Does nothing if the timer is not currently running.
   *
   * Note that there must be sufficient time between the this call finishing and
   * the scheduled event's termination. If this call is too close to when the
   * schedule event expires, this function may not complete before the hardware
   * calls the callback.
   *
   * @return status - success or failure
   */
  [[nodiscard]] status cancel() noexcept
  {
    return driver_cancel();
  }

  /**
   * @brief Schedule an callback be be executed after the delay time
   *
   * If this is called and the timer has already scheduled an event (in other
   * words, `is_running()` returns true), then the previous scheduled event will
   * be canceled and the new scheduled event will be started.
   *
   * If the delay time result in a tick period of 0, then the timer will execute
   * after 1 tick period. For example, if the tick period is 1ms and the
   * requested time delay is 500us, then the event will be scheduled for 1ms.
   *
   * If the tick period is 1ms and the requested time is 2.5ms then the event
   * will be scheduled after 2 tick periods or in 2ms.
   *
   * @param p_callback - callback function to be called when the timer expires
   * @param p_delay - the amount of time until the timer expires
   * @return status - success or failure
   * @throws out_of_bounds - if p_interval is greater than what can be cannot be
   * achieved
   */
  [[nodiscard]] status schedule(std::function<void(void)> p_callback,
                                hal::time_duration p_delay) noexcept
  {
    return driver_schedule(p_callback, p_delay);
  }

  virtual ~timer() = default;

private:
  virtual result<bool> driver_is_running() noexcept = 0;
  virtual status driver_cancel() noexcept = 0;
  virtual status driver_schedule(std::function<void(void)> p_callback,
                                 hal::time_duration p_delay) noexcept = 0;
};
/** @} */
}  // namespace hal
