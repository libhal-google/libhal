#pragma once

#include "../driver.hpp"

#include <chrono>
#include <cinttypes>
#include <cstddef>
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
   * @brief Set of controls for a timer.
   *
   */
  enum class controls
  {
    /**
     * @brief Control value to start the timer
     *
     */
    start,
    /**
     * @brief Control value to stop a timer
     *
     */
    stop,
    /**
     * @brief Control value to reset a timer. The timer shall remain in a
     * running or stopped state after this call. So an ongoing timer will
     * continue to count but will have its counter reset to zero if this control
     * is used. If a timer is stopped, then it shall be reset to zero, and stay
     * stopped.
     *
     */
    reset,
  };

  /**
   * @brief Type of timer
   *
   */
  enum class type
  {
    /**
     * @brief Once the timer's time has been reached an interrupt will fire and
     * the timer will be stopped.
     *
     */
    oneshot,
    /**
     * @brief Once the timer's time has been reached an interrupt will fire and
     * the timer will be reset and will begin counting again.
     *
     */
    continuous,
  };

  /**
   * @brief Determine if the timer is currently running
   *
   * @return boost::leaf::result<bool> - true if timer is currently running
   */
  virtual boost::leaf::result<bool> is_running() = 0;
  /**
   * @brief Control the state of the timer
   *
   * @param p_control - new state for the timer
   * @return boost::leaf::result<void>
   */
  virtual boost::leaf::result<void> control(controls p_control) = 0;
  /**
   * @brief Setup the timer and attach an interrupt to it
   *
   * When called this will, stop and reset the timer. To start the timer,
   * `control(controls::start)` must be called.
   *
   * @param p_callback - callback function to be called when the timer expires
   * @param p_interval - the amount of time before the timer expires
   * @param p_type - the type of timer this is
   * @return boost::leaf::result<void> - returns an error if not all parameters
   * could be met such as an interval smaller than is capable by hardware or an
   * unsupported timer type
   */
  virtual boost::leaf::result<void> attach_interrupt(
    std::function<void(void)> p_callback,
    std::chrono::nanoseconds p_interval,
    type p_type = type::continuous) = 0;
};
}  // namespace embed
