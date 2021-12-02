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
  /**
   * @brief Set of controls for a counter.
   *
   */
  enum class controls
  {
    /**
     * @brief Control value to start the counter
     *
     */
    start,
    /**
     * @brief Control value to stop a counter
     *
     */
    stop,
    /**
     * @brief Control value to reset a counter. The counter shall remain in a
     * running or stopped state after this call. So an ongoing counter will
     * continue to count but will have its counter reset to zero if this control
     * is used. If a counter is stopped, then it shall be reset to zero, and
     * stay stopped.
     *
     */
    reset,
  };

  /**
   * @brief Determine if the counter is currently running
   *
   * @return true counter is currently running
   * @return false counter is currently stopped
   */
  virtual bool is_running() = 0;
  /**
   * @brief Control the state of the counter
   *
   * @param p_control new state for the counter
   */
  virtual void control(controls p_control) = 0;
  /**
   * @brief Period for each count of the timer. For example a period of 1ms and
   * a count of 500 would mean that 500ms has elapse since the counter has been
   * started.
   *
   * @param p_period the amount of time each count should be.
   */
  virtual void period(std::chrono::nanoseconds p_period) = 0;
  /**
   * @brief Get the current period for the counter
   *
   * @return std::chrono::nanoseconds
   */
  virtual std::chrono::nanoseconds period() = 0;
  /**
   * @brief Get the count of the counter
   *
   * @return uint64_t current count
   */
  virtual uint64_t count() = 0;
};
} // namespace embed
