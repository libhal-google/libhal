/**
 * @file time.hpp
 * @brief Definitions and utilities for time related operations
 *
 */
#pragma once

#include <chrono>

#include "error.hpp"

namespace embed {
/// Definition of a delay handler
/// Delays execution of code based on the amount of time specified. Returns a
/// clock implementation specific error if there is one.
using delay_handler =
  boost::leaf::result<void>(std::chrono::nanoseconds p_delay_time);

/// Definition of an uptime handler.
/// Returns the amount of time since the clock has started or a clock
/// implementation specific error. The uptime resolution depends on the clock
/// driving it.
using uptime_handler = boost::leaf::result<std::chrono::nanoseconds>(void);

/// Definition of a timeout handler
/// The implementation of the timeout timer will generally use a counter, timer
/// or other timing system to indicate if a timeout condition has been
/// satisfied. When the timeout condition is satisfied this handler returns
/// true. If this handler is called and the condition is not satisfied then it
/// returns false. If an error has occurred during this operation then an
/// implementation defined error is returned.
using timeout_handler = boost::leaf::result<bool>(void);
}  // namespace embed