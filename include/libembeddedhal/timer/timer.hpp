#pragma once

#include <chrono>
#include <functional>

#include "../error.hpp"

namespace embed {
/**
 * @brief Timer hardware abstraction interface.
 *
 * Use this interface for devices and peripherals that have timer like
 * capabilities, such that, when a timer's time has expired, an
 * event/interrupt/signal is generated.
 *
 */
class timer
{
public:
  /**
   * @brief Error type indicating that the desired time delay is not achievable
   * with this timer.
   *
   * Usually this occurs if the time delay is too small or too big based on what
   * is possible with the driving frequency of the timer and along with any
   * prescalars before the counting register.
   *
   * <b>How to handle these errors:</b>
   *
   * - In cases where the program is scanning for the fastest delay, this is to
   *   be expected. The error will report the minimum possible delay which can
   *   then be assigned. Same for the maximum possible delay.
   *
   * - In most other cases, this is usually a bug in the code and cannot be
   *   handled in code and should be treated as such. Drivers using a timer, if
   *   they need an exact number will not be usable with this timer if it throws
   *   this error, which will either require another timer that can perform this
   *   work be used or increasing the clock rate fed into the timer in order to
   *   increase its frequency range.
   *
   */
  struct out_of_bounds
  {
    /// The invalid delay given to the schedule function.
    std::chrono::nanoseconds invalid;
    /// The minimum possible delay allowed.
    std::chrono::nanoseconds minimum;
    /// The maximum possible delay allowed.
    std::chrono::nanoseconds maximum;
  };

  /**
   * @brief Determine if the timer is currently running
   *
   * @return boost::leaf::result<bool> - true if timer is currently running
   * @return boost::leaf::result<bool> - driver specific error, if any.
   */
  [[nodiscard]] boost::leaf::result<bool> is_running()
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
   * @return boost::leaf::result<void> - driver specific error, if any.
   */
  [[nodiscard]] boost::leaf::result<void> clear() { return driver_clear(); }
  /**
   * @brief Schedule an callback to be called at a designated time/interval
   *
   * If this is called and the timer has already scheduled an event (in other
   * words, is_running() returns true), then the previous scheduled event will
   * be cleared and the new scheduled event will be started.
   *
   * @param p_callback - callback function to be called when the timer expires
   * @param p_delay - the amount of time before the timer expires
   * @return boost::leaf::result<void> - returns `delay_too_small` or
   * `delay_too_large` if p_interval cannot be reached.
   */
  [[nodiscard]] boost::leaf::result<void> schedule(
    std::function<void(void)> p_callback,
    std::chrono::nanoseconds p_delay)
  {
    return driver_schedule(p_callback, p_delay);
  }

private:
  virtual boost::leaf::result<bool> driver_is_running() = 0;
  virtual boost::leaf::result<void> driver_clear() = 0;
  virtual boost::leaf::result<void> driver_schedule(
    std::function<void(void)> p_callback,
    std::chrono::nanoseconds p_delay) = 0;
};
}  // namespace embed
