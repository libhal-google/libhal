#pragma once

#include <chrono>
#include <cstdint>

namespace embed {
/// The standard time durations in libembeddedhal is set to microseconds held
/// within an std::int32_t integer.
using time_duration = std::chrono::duration<std::int32_t, std::micro>;
}  // namespace embed
