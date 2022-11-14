/**
 * @file timeout.hpp
 * @brief Provides the hal::timeout type and utility functions that use that
 * type.
 *
 */
#pragma once

#include <functional>
#include <system_error>

#include "enum.hpp"
#include "error.hpp"

namespace hal {
/**
 * @addtogroup utility
 * @{
 */
/**
 * @brief Timeout is a callable object or function that signals to a procedure
 * that the procedure has exceeded its time allotment and should return control
 * to the calling function.
 *
 * @throws hal::timeout - when the timeout condition has been met.
 * @returns status - sets error flag set when timeout
 * condition has been met, otherwise returns success.
 */
using timeout_function = status(void);

template<class T>
concept timeout = std::convertible_to<T, std::function<timeout_function>>;

/**
 * @brief Worker is a callable object or function that can be repeatedly tried
 * with try_until
 *
 * @returns result<work_state> - sets error flag set when an error occurs,
 * otherwise returns work_state enum.
 */
using work_function = result<work_state>();

template<class T>
concept worker = std::convertible_to<T, std::function<work_function>>;

/**
 * @brief Delay the execution of the application or thread for a duration of
 * time.
 *
 * @tparam Timeout - timeout type
 * @param p_timeout - callable timeout object
 * @return status - success or failure
 */
[[nodiscard]] inline status delay(timeout auto p_timeout) noexcept
{
  bool waiting = true;

  // This lambda catches a `std::errc::timed_out` handle them by changing
  // `waiting` from true to false in order to break the while loop below.
  auto timeout_catcher =
    [&waiting](hal::match<std::errc, std::errc::timed_out> p_errc) -> status {
    (void)p_errc;
    // Simply change the waiting bool
    waiting = false;
    // return successful
    return {};
  };

  HAL_CHECK(hal::attempt(
    [&p_timeout]() -> status {
      // Continuously call p_callback until it either returns
      // `std::errc::timeout_out`
      while (true) {
        HAL_CHECK(p_timeout());
      }
      // Unreachable!
      return {};
    },
    timeout_catcher));

  return {};
}

/**
 * @brief Create a timeout that will never time out
 *
 * @return auto - callable that will never return timeout
 */
[[nodiscard]] inline auto never_timeout() noexcept
{
  return []() -> status { return {}; };
}

/**
 * @brief Repeatedly call a worker function until a timeout is achieved
 *
 * @param p_worker - worker function to repeatedly call
 * @param p_timeout - callable timeout object
 * @return result<work_state> - state of the worker function
 */
inline result<work_state> try_until(worker auto p_worker,
                                    timeout auto p_timeout) noexcept
{
  while (true) {
    auto state = HAL_CHECK(p_worker());
    if (state == work_state::failure || state == work_state::finished) {
      return state;
    }
    HAL_CHECK(p_timeout());
  }
  return new_error(std::errc::state_not_recoverable);
};
/** @} */
}  // namespace hal
