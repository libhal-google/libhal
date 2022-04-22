#pragma once

#include <units/isq/si/prefixes.h>
#include <units/isq/si/thermodynamic_temperature.h>

namespace embed {
struct nanokelvin
  : units::
      prefixed_unit<nanokelvin, units::isq::si::nano, units::isq::si::kelvin>
{};
struct microkelvin
  : units::
      prefixed_unit<microkelvin, units::isq::si::micro, units::isq::si::kelvin>
{};
struct millikelvin
  : units::
      prefixed_unit<millikelvin, units::isq::si::milli, units::isq::si::kelvin>
{};
struct kilokelvin
  : units::
      prefixed_unit<kilokelvin, units::isq::si::kilo, units::isq::si::kelvin>
{};
}  // namespace embed
