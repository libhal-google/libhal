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
  auto [frequency, current_count] = BOOST_LEAF_CHECK(p_counter.uptime());
  const auto cycles = BOOST_LEAF_CHECK(frequency.cycles_per(p_wait_duration));
  const auto end_count = current_count + cycles;

  while (end_count > current_count) {
    current_count = BOOST_LEAF_CHECK(p_counter.uptime()).count;
    continue;
  }

  return {};
}

/**
 * @brief Create a sleep function (specifically a lambda) that satisfies the
 * embed::sleep_function requirement.
 *
 * The sleep function will perform a busy wait in order to delay execution.
 *
 * @param p_counter - hardware counter driver
 * @return auto - lambda sleep function based on the counter
 */
inline auto to_sleep(counter& p_counter) noexcept
{
  auto function =
    [&p_counter](
      std::chrono::nanoseconds p_delay) -> boost::leaf::result<void> {
    BOOST_LEAF_CHECK(wait_for(p_counter, p_delay));
    return {};
  };

  static_assert(std::is_assignable<std::function<sleep_function>,
                                   decltype(function)>::value);

  return function;
}

/**
 * @brief Create an uptime function (specifically a lambda) that satisfies the
 * embed::uptime_function requirement.
 *
 * @param p_counter - hardware counter driver
 * @return auto - lambda uptime function based on the counter
 */
inline auto to_uptime(uptime_counter& p_counter) noexcept
{
  auto function =
    [&p_counter]() -> boost::leaf::result<std::chrono::nanoseconds> {
    return p_counter.uptime();
  };

  static_assert(std::is_assignable<std::function<uptime_function>,
                                   decltype(function)>::value);

  return function;
}
}  // namespace embed
