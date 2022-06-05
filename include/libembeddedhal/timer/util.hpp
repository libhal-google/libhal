/**
 * @file util.hpp
 * @brief Provide utility handlers for the timer interface
 */
#pragma once

#include "../time.hpp"
#include "interface.hpp"

namespace embed {
/**
 * @brief Create a timeout handler lambda from a timer that satisfies the
 * embed::timeout_handler definition.
 *
 * @param p_timer - hardware timer driver
 * @return auto - lambda timeout handler based on the timer
 */
[[nodiscard]] inline auto to_timeout(
  timer& p_timer,
  std::chrono::nanoseconds p_timeout) noexcept
{
  bool first_call = true;

  auto handler =
    [p_timeout, &p_timer, first_call]() mutable -> boost::leaf::result<bool> {
    if (first_call) {
      first_call = false;
      BOOST_LEAF_CHECK(p_timer.schedule([]() {}, p_timeout));
    }
    bool is_running = BOOST_LEAF_CHECK(p_timer.is_running());
    return !is_running;
  };

  static_assert(
    std::is_constructible_v<std::function<timeout_handler>, decltype(handler)>,
    "[INTERNAL] Callable must be convertible to a embed::uptime_handler");

  return handler;
}

/**
 * @brief Delay execution for this duration of time using a hardware timer
 * object.
 *
 * @param p_timer - hardware timer driver
 * @param p_duration - the amount of time to pause execution for
 * @return boost::leaf::result<void> - returns an error if a call to p_timer
 * uptime() results in an error otherwise, returns success.
 */
[[nodiscard]] inline boost::leaf::result<void> delay(
  timer& p_timer,
  std::chrono::nanoseconds p_duration) noexcept
{
  auto timeout = to_timeout(p_timer, p_duration);

  while (!BOOST_LEAF_CHECK(timeout())) {
    continue;
  }

  return {};
}

/**
 * @brief Create a delay handler lambda using an embed::timer that satisfies
 * the embed::delay_handler requirement.
 *
 * The delay handler will perform a busy wait in order to delay execution.
 *
 * @param p_timer - hardware timer driver
 * @return auto - lambda delay handler based on the timer
 */
[[nodiscard]] inline auto to_delay(timer& p_timer) noexcept
{
  auto handler =
    [&p_timer](std::chrono::nanoseconds p_delay) -> boost::leaf::result<void> {
    BOOST_LEAF_CHECK(delay(p_timer, p_delay));
    return {};
  };

  static_assert(
    std::is_constructible_v<std::function<delay_handler>, decltype(handler)>,
    "[INTERNAL] Callable must be convertible to a embed::uptime_handler");

  return handler;
}
}  // namespace embed
