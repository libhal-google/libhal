// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <chrono>
#include <cstdint>

/**
 * @brief The foundation of libhal containing, interfaces, utilities and soft
 * drivers.
 *
 */
namespace hal {
/// The standard time durations in libhal std::chrono::nanoseconds
using time_duration = std::chrono::nanoseconds;

/// Standard type for bytes in libhal. hal::byte has a number of annoyances that
/// results in more verbose code without much benefit and thus hal::byte was
/// created.
using byte = std::uint8_t;

/// Type for frequency represented in hertz.
using hertz = float;

/// Type for acceleration represented in the force applied by gravity at sea
/// level.
using g_force = float;

/// Type for current represented in amps.
using ampere = float;

/// Type for voltage represented in volts.
using volts = float;

/// Type for temperature represented in celsius.
using celsius = float;

/// Type for rotational velocity represented in RPMs.
using rpm = float;

/// Type for length represented in meters.
using meters = float;

/// Type for angle represented in degrees.
using degrees = float;

/// Type for magnetic field represented in gauss.
using gauss = float;

/**
 * @brief Set of possible pin mode resistor settings.
 *
 * See each enumeration to get more details about when and how these should be
 * used.
 *
 */
enum class pin_resistor
{
  /// No pull up. This will cause the pin to float. This may be desirable if the
  /// pin has an external resistor attached or if the signal is sensitive to
  /// external devices like resistors.
  none = 0,
  /// Pull the pin down to devices GND. This will ensure that the voltage read
  /// by the pin when there is no signal on the pin is LOW (or false).
  pull_down,
  /// See pull down explanation, but in this case the pin is pulled up to VCC,
  /// also called VDD on some systems.
  pull_up,
};

/**
 * @brief Namespace containing user defined literals for the hal standard units
 *
 */
namespace literals {

// =============================================================================
// Frequency
// =============================================================================

[[nodiscard]] consteval hertz operator""_Hz(long double p_value) noexcept
{
  return static_cast<float>(p_value);
}

[[nodiscard]] consteval hertz operator""_kHz(long double p_value) noexcept
{
  return static_cast<float>(p_value * std::kilo::num);
}

[[nodiscard]] consteval hertz operator""_MHz(long double p_value) noexcept
{
  return static_cast<float>(p_value * std::mega::num);
}

[[nodiscard]] consteval hertz operator""_GHz(long double p_value) noexcept
{
  return static_cast<float>(p_value * std::giga::num);
}

// =============================================================================
// G force
// =============================================================================

[[nodiscard]] consteval g_force operator""_g(long double p_value) noexcept
{
  return static_cast<float>(p_value);
}

// =============================================================================
// Ampere
// =============================================================================

[[nodiscard]] consteval ampere operator""_kA(long double p_value) noexcept
{
  return static_cast<float>(p_value * std::kilo::num);
}

[[nodiscard]] consteval ampere operator""_A(long double p_value) noexcept
{
  return static_cast<float>(p_value);
}

[[nodiscard]] consteval ampere operator""_mA(long double p_value) noexcept
{
  return static_cast<float>(p_value / std::milli::den);
}

[[nodiscard]] consteval ampere operator""_uA(long double p_value) noexcept
{
  return static_cast<float>(p_value / std::micro::den);
}

// =============================================================================
// Voltage
// =============================================================================

[[nodiscard]] consteval volts operator""_kV(long double p_value) noexcept
{
  return static_cast<float>(p_value * std::kilo::num);
}

[[nodiscard]] consteval volts operator""_V(long double p_value) noexcept
{
  return static_cast<float>(p_value);
}

[[nodiscard]] consteval volts operator""_mV(long double p_value) noexcept
{
  return static_cast<float>(p_value / std::milli::den);
}

[[nodiscard]] consteval volts operator""_uV(long double p_value) noexcept
{
  return static_cast<float>(p_value / std::micro::den);
}

// =============================================================================
// Temperature
// =============================================================================

[[nodiscard]] consteval celsius operator""_C(long double p_value) noexcept
{
  return static_cast<float>(p_value);
}

[[nodiscard]] consteval celsius operator""_F(long double p_value) noexcept
{
  p_value = (p_value - 32.0L) * (5.0L / 9.0L);
  return static_cast<float>(p_value);
}

[[nodiscard]] consteval celsius operator""_K(long double p_value) noexcept
{
  return static_cast<float>(p_value - 273.15L);
}

// =============================================================================
// Rotational Velocity
// =============================================================================

[[nodiscard]] consteval rpm operator""_rpm(long double p_value) noexcept
{
  return static_cast<float>(p_value);
}

[[nodiscard]] consteval rpm operator""_deg_per_sec(long double p_value) noexcept
{
  return static_cast<float>(p_value / 6.0L);
}

// =============================================================================
// Angle
// =============================================================================

[[nodiscard]] consteval degrees operator""_deg(long double p_value) noexcept
{
  return static_cast<float>(p_value);
}

// =============================================================================
// Lengths
// =============================================================================

[[nodiscard]] consteval meters operator""_um(long double p_value) noexcept
{
  return static_cast<float>(p_value / std::micro::den);
}

[[nodiscard]] consteval meters operator""_mm(long double p_value) noexcept
{
  return static_cast<float>(p_value / std::milli::den);
}

[[nodiscard]] consteval meters operator""_m(long double p_value) noexcept
{
  return static_cast<float>(p_value);
}

[[nodiscard]] consteval meters operator""_km(long double p_value) noexcept
{
  return static_cast<float>(p_value * std::kilo::num);
}

[[nodiscard]] consteval meters operator""_inch(long double p_value) noexcept
{
  constexpr long double inch_to_meter = 0.0254L;
  return static_cast<float>(p_value * inch_to_meter);
}

[[nodiscard]] consteval meters operator""_yards(long double p_value) noexcept
{
  constexpr long double yard_to_meter = 0.9144L;
  return static_cast<float>(p_value * yard_to_meter);
}

[[nodiscard]] consteval meters operator""_miles(long double p_value) noexcept
{
  constexpr long double miles_to_meter = 1609.344L;
  return static_cast<float>(p_value * miles_to_meter);
}
}  // namespace literals

// Make user defined namespaces available to any library within the hal
// namespace
using namespace literals;
}  // namespace hal
