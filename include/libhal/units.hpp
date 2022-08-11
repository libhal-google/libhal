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
// TODO: When hal::frequency is eliminated, remove this so that literals can be
// used easily within classes & interfaces.
namespace experimental::literals {

// =============================================================================
// Frequency
// =============================================================================

[[nodiscard]] consteval hertz operator""_Hz(long double p_value) noexcept
{
  return static_cast<config::float_type>(p_value);
}

[[nodiscard]] consteval hertz operator""_kHz(long double p_value) noexcept
{
  return static_cast<config::float_type>(p_value) * std::kilo::num;
}

[[nodiscard]] consteval hertz operator""_MHz(long double p_value) noexcept
{
  return static_cast<config::float_type>(p_value) * std::mega::num;
}

[[nodiscard]] consteval hertz operator""_GHz(long double p_value) noexcept
{
  return static_cast<config::float_type>(p_value) * std::giga::num;
}

// =============================================================================
// G force
// =============================================================================

[[nodiscard]] consteval g_force operator""_g(long double p_value) noexcept
{
  return static_cast<config::float_type>(p_value);
}

// =============================================================================
// Lengths
// =============================================================================

[[nodiscard]] consteval meters operator""_um(long double p_value) noexcept
{
  return static_cast<config::float_type>(p_value) / std::micro::den;
}

[[nodiscard]] consteval meters operator""_mm(long double p_value) noexcept
{
  return static_cast<config::float_type>(p_value) / std::milli::den;
}

[[nodiscard]] consteval meters operator""_m(long double p_value) noexcept
{
  return static_cast<config::float_type>(p_value);
}

[[nodiscard]] consteval meters operator""_km(long double p_value) noexcept
{
  return static_cast<config::float_type>(p_value) * std::kilo::num;
}

[[nodiscard]] consteval meters operator""_inch(long double p_value) noexcept
{
  constexpr config::float_type inch_to_meter = 0.0254;
  return static_cast<config::float_type>(p_value) * inch_to_meter;
}

[[nodiscard]] consteval meters operator""_yards(long double p_value) noexcept
{
  constexpr config::float_type yard_to_meter = 0.9144;
  return static_cast<config::float_type>(p_value) * yard_to_meter;
}

[[nodiscard]] consteval meters operator""_miles(long double p_value) noexcept
{
  constexpr config::float_type miles_to_meter = 1609.344;
  return static_cast<config::float_type>(p_value) * miles_to_meter;
}
}  // namespace experimental::literals

/** @} */
}  // namespace hal
