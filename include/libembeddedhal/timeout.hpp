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
 * @returns boost::leaf::result<void> - sets error flag set when timeout
 * condition has been met, otherwise returns success.
 */
using timeout = boost::leaf::result<void>(void);

/**
 * @brief Delay the execution of the application or thread for a duration of
 * time.
 *
 * @tparam Timeout - timeout type
 * @param p_timeout - callable timeout object
 * @return boost::leaf::result<void> - success
 */
template<typename Timeout = std::function<hal::timeout>>
[[nodiscard]] inline boost::leaf::result<void> delay(Timeout p_timeout) noexcept
{
  bool waiting = true;

  // This lambda catches a `std::errc::timed_out` handle them by changing
  // `waiting` from true to false in order to break the while loop below.
  auto timeout_catcher =
    [&waiting](boost::leaf::match<std::errc, std::errc::timed_out> p_errc)
    -> boost::leaf::result<void> {
    (void)p_errc;
    // Simply change the waiting bool
    waiting = false;
    // return successful
    return {};
  };

  while (waiting) {
    // Rethrow any error that isn't `std::errc::timed_out`
    BOOST_LEAF_CHECK(boost::leaf::try_handle_some(p_timeout, timeout_catcher));
  }

  return {};
}

/**
 * @brief Create a timeout that will never time out
 *
 * @return auto - callable that will never return timeout
 */
[[nodiscard]] inline auto never_timeout() noexcept
{
  return []() -> boost::leaf::result<void> { return {}; };
}
/** @} */
}  // namespace hal
