#pragma once

#include <chrono>
#include <functional>

#include "error.hpp"

namespace embed {
/// Definition of a sleep function
/// Delays execution of code based on the amount of time specified. Returns a
/// clock implementation specific error if there is one.
using sleep_function =
  boost::leaf::result<void>(std::chrono::nanoseconds p_sleep_time);
/// Definition of an uptime function.
/// Returns the amount of time since the clock has started or a clock
/// implementation specific error. The uptime resolution depends on the clock
/// driving it.
using uptime_function = boost::leaf::result<std::chrono::nanoseconds>(void);
}  // namespace embed