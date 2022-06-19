/**
 * @file timeout.hpp
 * @brief Provides the embed::timeout type and utility functions that use that
 * type.
 *
 */
#pragma once

#include <functional>

#include "error.hpp"

namespace embed {
/**
 * @brief Timeout is a callable object or function that signals to a procedure
 * that the procedure has exceeded its time allotment and should return control
 * to the calling function.
 *
 * @throws embed::timeout - when the timeout condition has been met.
 * @returns boost::leaf::result<void> - sets error flag set when timeout
 * condition has been met, otherwise returns success.
 */
using timeout = boost::leaf::result<void>(void) noexcept;

/**
 * @brief Delay the execution of the application or thread for a duration of
 * time.
 *
 * @tparam Timeout - timeout type
 * @param p_timeout_function - timeout callable
 * @return boost::leaf::result<void> - returns an error if an error other than
 * embed::error::timeout is returned, otherwise returns success.
 */
template<typename Timeout = std::function<embed::timeout>>
[[nodiscard]] inline boost::leaf::result<void> delay(
  Timeout p_timeout_function) noexcept
{
  bool waiting = true;

  while (waiting) {
    // If any error other than `embed::error::timeout` is thrown, return an
    // error flag from this function.
    BOOST_LEAF_CHECK(boost::leaf::try_handle_some(
      // Execute the timeout function and check if it returns an error
      [&p_timeout_function]() -> boost::leaf::result<void> {
        return p_timeout_function();
      },
      // Catch `embed::error::timeout` errors and handle them by changing
      // `waiting` from true to false.
      [&waiting]([[maybe_unused]] embed::error::timeout p_timeout_error)
        -> boost::leaf::result<void> {
        waiting = false;
        return {};
      }));
  }

  return {};
}
}  // namespace embed
