#pragma once

#include "../driver.hpp"

#include <chrono>
#include <cinttypes>

namespace embed {
/**
 * @brief Counter hardware abstraction interface. Use this interface for devices
 * and peripherals that have counting capabilities. Such devices can count up or
 * down. Useful for keeping time. Expect counters to overflow when they reach
 * the end of counting register's limits.
 *
 */
class counter : public driver<>
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

  /// default constructor
  counter() = default;
  /// Explicitly delete copy constructor to prevent slicing
  counter(const counter& p_other) = delete;
  /// Explicitly delete assignment operator to prevent slicing
  counter& operator=(const counter& p_other) = delete;
  /// Destroy the object
  virtual ~counter() = default;

  /**
   * @brief Determine if the counter is currently running
   *
   * @return boost::leaf::result<bool> - true if the counter is currently
   * running.
   */
  virtual boost::leaf::result<bool> is_running() = 0;
  /**
   * @brief Control the state of the counter
   *
   * @param p_control - new state for the counter
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  virtual boost::leaf::result<void> control(controls p_control) = 0;
  /**
   * @brief Period for each count of the timer. For example a period of 1ms and
   * a count of 500 would mean that 500ms has elapse since the counter has been
   * started.
   *
   * @param p_period - the amount of time in nanoseconds each count should be
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  virtual boost::leaf::result<void> period(
    std::chrono::nanoseconds p_period) = 0;
  /**
   * @brief Get the current period for the counter
   *
   * @return boost::leaf::result<std::chrono::nanoseconds> - the period of time
   * for each count.
   */
  virtual boost::leaf::result<std::chrono::nanoseconds> period() = 0;
  /**
   * @brief Get the count of the counter
   *
   * @return boost::leaf::result<uint64_t> - the current count
   */
  virtual boost::leaf::result<uint64_t> count() = 0;
};
}  // namespace embed
