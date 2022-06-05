/**
 * @file util.hpp
 * @brief Provide utility handlers for the counter interface
 */
#pragma once

#include "../time.hpp"
#include "interface.hpp"
#include "overflow_counter.hpp"

namespace embed {
/**
 * @brief Create an uptime handler lambda using an embed::counter that satisfies
 * the embed::uptime_handler requirement.
 *
 * @param p_counter - hardware counter driver
 * @return auto - lambda uptime handler based on the counter
 */
[[nodiscard]] inline auto to_uptime(counter& p_counter) noexcept
{
  overflow_counter counter(p_counter);

  auto handler =
    [counter]() mutable -> boost::leaf::result<std::chrono::nanoseconds> {
    return counter.uptime();
  };

  static_assert(
    std::is_constructible_v<std::function<uptime_handler>, decltype(handler)>,
    "[INTERNAL] Callable must be convertible to a embed::uptime_handler");

  return handler;
}

/**
 * @brief Create a timeout handler lambda from a counter that satisfies the
 * embed::timeout_handler definition.
 *
 * @param p_counter - hardware counter driver
 * @return auto - lambda timeout handler based on the counter
 */
[[nodiscard]] inline auto to_timeout(
  counter& p_counter,
  std::chrono::nanoseconds p_timeout) noexcept
{
  auto previous_count = 0;
  bool first_call = true;
  std::int64_t cycles = 0;

  auto handler = [cycles,
                  p_timeout,
                  &p_counter,
                  previous_count,
                  first_call]() mutable -> boost::leaf::result<bool> {
    if (first_call) {
      const auto [frequency, count] = BOOST_LEAF_CHECK(p_counter.uptime());
      previous_count = count;
      first_call = false;
      cycles = BOOST_LEAF_CHECK(frequency.cycles_per(p_timeout));
    }

    if (cycles <= 0) {
      return true;
    }

    auto current_count = BOOST_LEAF_CHECK(p_counter.uptime()).count;
    std::uint32_t delta_count = current_count - previous_count;
    previous_count = current_count;
    cycles -= delta_count;

    return false;
  };

  static_assert(
    std::is_constructible_v<std::function<timeout_handler>, decltype(handler)>,
    "[INTERNAL] Callable must be convertible to a embed::uptime_handler");

  return handler;
}

/**
 * @brief Delay execution for this duration of time using a hardware counter
 * object.
 *
 * @param p_counter - hardware counter driver
 * @param p_duration - the amount of time to pause execution for
 * @return boost::leaf::result<void> - returns an error if a call to p_counter
 * uptime() results in an error otherwise, returns success.
 */
[[nodiscard]] inline boost::leaf::result<void> delay(
  counter& p_counter,
  std::chrono::nanoseconds p_duration) noexcept
{
  auto timeout = to_timeout(p_counter, p_duration);

  while (!BOOST_LEAF_CHECK(timeout())) {
    continue;
  }

  return {};
}

/**
 * @brief Create a delay handler lambda using an embed::counter that satisfies
 * the embed::delay_handler requirement.
 *
 * The delay handler will perform a busy wait in order to delay execution.
 *
 * @param p_counter - hardware counter driver
 * @return auto - lambda delay handler based on the counter
 */
[[nodiscard]] inline auto to_delay(counter& p_counter) noexcept
{
  auto handler =
    [&p_counter](
      std::chrono::nanoseconds p_delay) -> boost::leaf::result<void> {
    BOOST_LEAF_CHECK(delay(p_counter, p_delay));
    return {};
  };

  static_assert(
    std::is_constructible_v<std::function<delay_handler>, decltype(handler)>,
    "[INTERNAL] Callable must be convertible to a embed::uptime_handler");

  return handler;
}
}  // namespace embed
