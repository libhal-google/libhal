#pragma once

#include <chrono>
#include <cstdint>

#include "config.hpp"
#include "error.hpp"
#include "math.hpp"

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

/**
 * @brief Calculate the number of cycles of this frequency within the time
 * duration. This function is meant for timers to determine how many count
 * cycles are needed to reach a particular time duration at this frequency.
 *
 * @param p_source - source frequency
 * @param p_duration - the amount of time to convert to cycles
 * @return std::int64_t - number of cycles
 */
[[nodiscard]] constexpr std::int64_t cycles_per(
  hertz p_source,
  hal::time_duration p_duration) noexcept
{
  // Full Equation:
  //                              / ratio_num \_
  //   frequency_hz * |period| * | ----------- |  = cycles
  //                              \ ratio_den /
  //
  // std::chrono::nanoseconds::period::num == 1
  // std::chrono::nanoseconds::period::den == 1,000,000

  const auto denominator = decltype(p_duration)::period::den;
  const auto cycle_count = (p_duration.count() * p_source) / denominator;

  return static_cast<std::int64_t>(cycle_count);
}

/**
 * @brief Calculates and returns the wavelength in seconds.
 *
 * @tparam Period - desired period (defaults to std::femto for femtoseconds).
 * @param p_source - source frequency to convert to wavelength
 * @return std::chrono::duration<int64_t, Period> - time based wavelength of
 * the frequency.
 */
template<typename Period>
constexpr std::chrono::duration<int64_t, Period> wavelength(hertz p_source)
{
  if (equals(p_source, 0.0)) {
    return std::chrono::duration<int64_t, Period>(0);
  }
  auto duration = (1.0f / p_source) * Period::den;
  return std::chrono::duration<int64_t, Period>(static_cast<int64_t>(duration));
}

/**
 * @brief Calculates and returns the wavelength in seconds as a float.
 *
 * @tparam float_t - float type
 * @tparam Period - desired period
 * @param p_source - source frequency to convert to wavelength
 * @return constexpr float_t - float representation of the time based wavelength
 * of the frequency.
 */
template<std::floating_point float_t = config::float_type>
constexpr float_t wavelength(hertz p_source)
{
  if (equals(p_source, 0.0)) {
    return float_t(0);
  }
  auto duration = (1.0f / p_source);
  return float_t(duration);
}

/**
 * @brief Calculate the amount of time it takes a frequency to oscillate a
 * number of cycles.
 *
 * @param p_source - the frequency to compute the cycles from
 * @param p_cycles - number of cycles within the time duration
 * @return std::chrono::nanoseconds - time duration based on this frequency
 * and the number of cycles
 */
[[nodiscard]] inline result<std::chrono::nanoseconds> duration_from_cycles(
  hertz p_source,
  std::uint32_t p_cycles) noexcept
{
  // Full Equation (based on the equation in cycles_per()):
  //
  //
  //                /    cycles * ratio_den    \_
  //   |period| =  | ---------------------------|
  //                \ frequency_hz * ratio_num /
  //
  constexpr auto ratio_den = std::chrono::nanoseconds::period::den;
  constexpr auto ratio_num = std::chrono::nanoseconds::period::num;
  constexpr auto int_min = std::numeric_limits<std::int64_t>::min();
  constexpr auto int_max = std::numeric_limits<std::int64_t>::max();
  constexpr auto float_int_min = static_cast<config::float_type>(int_min);
  constexpr auto float_int_max = static_cast<config::float_type>(int_max);

  const auto source = std::abs(p_source);
  auto nanoseconds = (p_cycles * ratio_den) / (source * ratio_num);

  if (float_int_min <= nanoseconds && nanoseconds <= float_int_max) {
    return std::chrono::nanoseconds(static_cast<std::int64_t>(nanoseconds));
  }
  return new_error(std::errc::result_out_of_range);
}
/** @} */
}  // namespace hal
