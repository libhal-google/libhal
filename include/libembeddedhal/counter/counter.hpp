#pragma once

#include <chrono>
#include <cstdint>

#include "../error.hpp"
#include "../frequency.hpp"

namespace embed {
/**
 * @brief Counter hardware abstraction interface. Use this interface for devices
 * and peripherals that have counting capabilities. Such devices can count up or
 * down. Useful for keeping time. Expect counters to overflow when they reach
 * the end of counting register's limits.
 *
 */
class counter
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
   * @brief Control the state of the counter
   *
   * @param p_control - new state for the counter
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  [[nodiscard]] virtual boost::leaf::result<void> control(controls p_control)
  {
    return driver_control(p_control);
  }
  /**
   * @brief Determine if the counter is currently running.
   *
   * If attempting to check if the counter is running results in an error,
   * then treat that as if the counter is not active and return false.
   *
   * @return bool - true if the counter is currently running.
   */
  [[nodiscard]] virtual bool is_running() { return driver_is_running(); }
  /**
   * @brief Get the uptime of the counter since it has started.
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
   * @return std::chrono::nanoseconds - the current uptime since the counter has
   * started.
   */
  [[nodiscard]] std::chrono::nanoseconds uptime() { return driver_uptime(); }

private:
  virtual boost::leaf::result<void> driver_control(controls p_control) = 0;
  virtual bool driver_is_running() = 0;
  virtual std::chrono::nanoseconds driver_uptime() = 0;
};
}  // namespace embed
