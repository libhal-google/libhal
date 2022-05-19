#pragma once

#include "../time.hpp"
#include "interface.hpp"
#include "uptime_counter.hpp"

namespace embed {
/**
 * @brief pause execution for this duration of time using a hardware counter
 * object.
 *
 * @param p_counter - hardware counter driver
 * @param p_wait_duration - the amount of time to pause execution for
 */
inline boost::leaf::result<void> wait_for(
  counter& p_counter,
  std::chrono::nanoseconds p_wait_duration) noexcept
{
  const auto [frequency, start_count] = BOOST_LEAF_CHECK(p_counter.uptime());
  const auto cycles = BOOST_LEAF_CHECK(frequency.cycles_per(p_wait_duration));
  const auto end_count = start_count + cycles;

  while (end_count >= BOOST_LEAF_CHECK(p_counter.uptime()).count) {
    continue;
  }
}
/**
 * @brief Use this counter as the global sleep function.
 *
 * The counter must be infailable meaning that frequency() and uptime()
 * must never return an error, otherwise the program will abort.
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
 * @brief Use this counter as the global sleep function.
 *
 * The counter must be infailable meaning that frequency() and uptime()
 * must never return an error, otherwise the program will abort.
 *
 * @param p_counter - hardware counter driver
 */
inline void set_as_global_uptime(uptime_counter& p_counter) noexcept
{
  this_thread::set_global_uptime([&p_counter]() -> std::chrono::nanoseconds {
    return p_counter.uptime().value();
  });
}
}  // namespace embed
