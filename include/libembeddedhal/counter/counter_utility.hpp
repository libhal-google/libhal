#pragma once

#include "../time.hpp"
#include "counter.hpp"

namespace embed {
/**
 * @brief Reset counter
 *
 * @param p_counter - hardware counter driver
 * @return boost::leaf::result<void> - any errors that occurred when attempting
 * to reset counter.
 */
[[nodiscard]] inline boost::leaf::result<void> reset(counter& p_counter)
{
  return p_counter.control(counter::controls::reset);
}
/**
 * @brief Start counter
 *
 * @param p_counter - hardware counter driver
 * @return boost::leaf::result<void> - any errors that occurred when attempting
 * to start counter.
 */
[[nodiscard]] inline boost::leaf::result<void> start(counter& p_counter)
{
  return p_counter.control(counter::controls::start);
}
/**
 * @brief Stop counter
 *
 * @param p_counter - hardware counter driver
 * @return boost::leaf::result<void> - any errors that occurred when attempting
 * to stop counter.
 */
[[nodiscard]] inline boost::leaf::result<void> stop(counter& p_counter)
{
  return p_counter.control(counter::controls::stop);
}
/**
 * @brief pause execution for this duration of time using a hardware counter
 * object.
 *
 * @param p_counter - hardware counter driver
 * @param p_wait_duration - the amount of time to pause execution for
 */
inline void wait_for(counter& p_counter,
                     std::chrono::nanoseconds p_wait_duration)
{
  const auto start_time = p_counter.uptime();
  const auto end_time = start_time + p_wait_duration;

  while (end_time >= p_counter.uptime()) {
    continue;
  }
}
/**
 * @brief Use this counter as the global sleep function
 *
 * @param p_counter - hardware counter driver
 */
inline void set_as_global_sleep(counter& p_counter) noexcept
{
  this_thread::set_global_sleep([&p_counter](std::chrono::nanoseconds p_delay) {
    wait_for(p_counter, p_delay);
  });
}
/**
 * @brief Use this counter as the global sleep function
 *
 * @param p_counter - hardware counter driver
 */
inline void set_as_global_uptime(counter& p_counter) noexcept
{
  this_thread::set_global_uptime(
    [&p_counter]() -> std::chrono::nanoseconds { return p_counter.uptime(); });
}
}  // namespace embed
