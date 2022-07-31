/**
 * @file timeout.hpp
 * @brief Provides the hal::timeout type and utility functions that use that
 * type.
 *
 */
#pragma once

#include <functional>
#include <system_error>

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
using timeout = status(void);

/**
 * @brief Delay the execution of the application or thread for a duration of
 * time.
 *
 * @tparam Timeout - timeout type
 * @param p_timeout - callable timeout object
 * @return status - success
 */
template<typename Timeout = std::function<hal::timeout>>
[[nodiscard]] inline status delay(Timeout p_timeout) noexcept
{
  bool waiting = true;

  // This lambda catches a `std::errc::timed_out` handle them by changing
  // `waiting` from true to false in order to break the while loop below.
  auto timeout_catcher =
    [&waiting](
      boost::leaf::match<std::errc, std::errc::timed_out> p_errc) -> status {
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
/** @} */
}  // namespace hal
