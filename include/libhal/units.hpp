#pragma once

#include <chrono>
#include <cstdint>

namespace hal {
/**
 * @addtogroup utility
 * @{
 */

/// The standard time durations in libhal is set to microseconds held
/// within an std::int32_t integer.
using time_duration = std::chrono::duration<std::int32_t, std::micro>;

/// Standard type for bytes in libhal. hal::byte has a number of annoyances that
/// results in more verbose code without much benefit and thus hal::byte was
/// created.
using byte = std::uint8_t;
/** @} */
}  // namespace hal
