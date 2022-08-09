#pragma once

#include <chrono>
#include <cstdint>

#include "config.hpp"

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

/// Type for frequency represented in hertz
using hertz = config::float_type;

/// Type for acceleration represented in the force applied by gravity at sea
/// level.
using g_force = config::float_type;

/// Type for length represented in meters.
using meters = config::float_type;

// Namespace containing user defined literals for the hal standard units
namespace experimental::literals {
[[nodiscard]] consteval hertz operator""_Hz(unsigned long long p_value) noexcept
{
  return static_cast<config::float_type>(p_value);
}
[[nodiscard]] consteval hertz operator""_kHz(
  unsigned long long p_value) noexcept
{
  return static_cast<config::float_type>(p_value) * std::kilo::num;
}
[[nodiscard]] consteval hertz operator""_MHz(
  unsigned long long p_value) noexcept
{
  return static_cast<config::float_type>(p_value) * std::mega::num;
}
[[nodiscard]] consteval g_force operator""_g(
  unsigned long long p_value) noexcept
{
  return static_cast<config::float_type>(p_value);
}
[[nodiscard]] consteval meters operator""_um(
  unsigned long long p_value) noexcept
{
  return static_cast<config::float_type>(p_value) / std::micro::den;
}
[[nodiscard]] consteval meters operator""_mm(
  unsigned long long p_value) noexcept
{
  return static_cast<config::float_type>(p_value) / std::milli::den;
}
[[nodiscard]] consteval meters operator""_m(unsigned long long p_value) noexcept
{
  return static_cast<config::float_type>(p_value);
}
[[nodiscard]] consteval meters operator""_km(
  unsigned long long p_value) noexcept
{
  return static_cast<config::float_type>(p_value) * std::kilo::num;
}
}  // namespace experimental::literals

/** @} */
}  // namespace hal
