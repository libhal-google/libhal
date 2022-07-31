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

/** @} */
}  // namespace hal
