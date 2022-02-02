#pragma once

#include "../driver.hpp"

#include <chrono>
#include <cinttypes>

namespace embed {
/// Generic settings for a hardware counting devices.
struct counter_settings
{
  /// The target time increment of the counter
  std::chrono::nanoseconds clock_period = std::chrono::milliseconds(1);
};
/**
 * @brief Counter hardware abstraction interface. Use this interface for devices
 * and peripherals that have counting capabilities. Such devices can count up or
 * down. Useful for keeping time. Expect counters to overflow when they reach
 * the end of counting register's limits.
 *
 */
class counter : public driver<counter_settings>
{
public:
  /// Set of controls for a counter.
  enum class controls
  {
    /// Start the counter
    start,
    /// Stop a counter
    stop,
    /// Control value to reset a counter. The counter shall remain in a
    /// running or stopped state after this call. So an ongoing counter will
    /// continue to count but will have its counter reset to zero if this
    /// control is used. If a counter is stopped, then it shall be reset to
    /// zero, and stay stopped.
    reset,
  };

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
   * @brief Control the state of the counter
   *
   * @param p_control - new state for the counter
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  virtual boost::leaf::result<void> control(controls p_control) = 0;
  /**
   * @brief Determine if the counter is currently running.
   *
   * If attempting to check if the counter is running results in an error,
   * then treat that as if the counter is not active and return false.
   *
   * @return bool - true if the counter is currently running.
   */
  virtual bool is_running() = 0;
  /**
   * @brief Get the current count of the counter
   *
   * Most counters are only support 32-bits or lower. In this case there are
   * multiple way to increase the bit width of the counter:
   *
   *  1. Use the overflow_count class to detect overflows each time the count is
   *     called. In cases where this is used, the counter::count() must be
   *     called often enough to detect overflows
   *  2. Use the overflow_count class with overflow interrupt. This will
   *     eliminate the need to call the count often enough to detect the
   *     overflows.
   *
   * @return uint64_t - the current count
   */
  virtual uint64_t count() = 0;
};
}  // namespace embed
