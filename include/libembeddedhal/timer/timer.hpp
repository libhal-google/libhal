#pragma once

#include "../driver.hpp"

#include <chrono>
#include <functional>

namespace embed {
/**
 * @brief Timer hardware abstraction interface.
 *
 * Use this interface for devices and peripherals that have timer like
 * capabilities, such that, when a timer's time has expired, an
 * event/interrupt/signal is generated.
 *
 */
class timer : public driver<>
{
public:
  /**
   * @brief Error type indicating that the desired time delay is not achievable
   * with this timer.
   *
   * Usually this occurs if the time delay is too small relative to the driving
   * frequency of the timer and along with any prescalars before the counting
   * register.
   *
   * <b>How to handle these errors:</b>
   *
   * - In cases where the program is scanning for the fastest delay, this is to
   *   be expected. The error will report the minimum possible delay which can
   *   then be assigned. Same for the maximum possible delay.
   *
   * - In most other cases, this is usually a bug in the code and cannot be
   *   handled in code and should be treated as such. Drivers using a timer, if
   *   they need an exact nunber will not be usable with this timer if it throws
   *   this error, which will either require another timer that can perform this
   *   work be used or increasing the clock rate fed into the timer in order to
   *   increase its frequency range.
   *
   */
  struct delay_too_small
  {
    /// The invalid delay given to the schedule function
    std::chrono::nanoseconds invalid;
    /// The minimum possible delay allowed.
    std::chrono::nanoseconds minimum;
  };

  /**
   * @brief Error type indicating that the desired delay time is above what can
   * be achieved by the timer.
   *
   * Usually occurs when the driving frequency, prescalar values and the size of
   * the timer registers are too small for the delay for a particular timer.
   *
   * <b>How to handle these errors:</b>
   *
   * - Usually this is a bug in the program. One way to resolve this in the
   *   application is to drive down the input frequency to the timer in order to
   *   make each count longer.
   *
   */
  struct delay_too_large
  {
    /// The invalid delay given to the schedule function
    std::chrono::nanoseconds invalid;
    /// The maximum possible delay allowed.
    std::chrono::nanoseconds maximum;
  };

  /**
   * @brief Determine if the timer is currently running
   *
   * @return boost::leaf::result<bool> - true if timer is currently running
   * @return boost::leaf::result<bool> - driver specific error, if any.
   */
  virtual boost::leaf::result<bool> is_running() = 0;

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
  virtual boost::leaf::result<void> clear() = 0;

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
  virtual boost::leaf::result<void> schedule(
    std::function<void(void)> p_callback,
    std::chrono::nanoseconds p_delay) = 0;
};
}  // namespace embed
