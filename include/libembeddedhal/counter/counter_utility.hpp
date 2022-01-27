#pragma once

#include "../time.hpp"
#include "counter.hpp"

namespace embed {
/**
 * @brief Reset counter
 *
 * @param p_counter - counter object
 * @return boost::leaf::result<void> - any errors that occurred when attempting
 * to reset counter.
 */
inline boost::leaf::result<void> reset(counter& p_counter)
{
  return p_counter.control(counter::controls::reset);
}
/**
 * @brief Start counter
 *
 * @param p_counter - counter object
 * @return boost::leaf::result<void> - any errors that occurred when attempting
 * to start counter.
 */
inline boost::leaf::result<void> start(counter& p_counter)
{
  return p_counter.control(counter::controls::start);
}
/**
 * @brief Stop counter
 *
 * @param p_counter - counter object
 * @return boost::leaf::result<void> - any errors that occurred when attempting
 * to stop counter.
 */
inline boost::leaf::result<void> stop(counter& p_counter)
{
  return p_counter.control(counter::controls::stop);
}
/**
 * @brief Get uptime since the counter has started
 *
 * @param p_counter - counter object
 * @return std::chrono::nanoseconds - time in nanoseconds
 */
inline std::chrono::nanoseconds uptime(counter& p_counter)
{
  return p_counter.initialized_settings().clock_period * p_counter.count();
}
/**
 * @brief pause execution for this duration of time using a hardware counter
 * object.
 *
 * @param p_counter - counter object
 * @param p_duration - time duration
 */
inline void sleep_for(counter& p_counter, std::chrono::nanoseconds p_duration)
{
  const auto clock_period = p_counter.initialized_settings().clock_period;
  const auto cycle_count = p_duration / clock_period;
  const auto start_time = p_counter.count();
  const auto end_time = start_time + cycle_count;

  while (end_time >= p_counter.count()) {
    continue;
  }
}
/**
 * @brief Use this counter as the global sleep function
 *
 * @param p_counter - counter object
 */
inline void set_as_global_sleep(counter& p_counter)
{
  this_thread::set_global_sleep([&p_counter](std::chrono::nanoseconds p_delay) {
    sleep_for(p_counter, p_delay);
  });
}
/**
 * @brief Use this counter as the global sleep function
 *
 * @param p_counter - counter object
 */
inline void set_as_global_uptime(counter& p_counter)
{
  this_thread::set_global_uptime(
    [&p_counter]() -> std::chrono::nanoseconds { return uptime(p_counter); });
}
}  // namespace embed
