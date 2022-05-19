#pragma once

#include <cstdio>

#include <units/isq/si/prefixes.h>
#include <units/isq/si/thermodynamic_temperature.h>

namespace embed {
struct nanokelvin
  : units::
      prefixed_unit<nanokelvin, units::isq::si::nano, units::isq::si::kelvin>
{};
template<units::Representation Rep = std::int64_t>
using nK = units::isq::si::thermodynamic_temperature<nanokelvin, Rep>;

struct microkelvin
  : units::
      prefixed_unit<microkelvin, units::isq::si::micro, units::isq::si::kelvin>
{};

template<units::Representation Rep = std::int64_t>
using uK = units::isq::si::thermodynamic_temperature<microkelvin, Rep>;

struct millikelvin
  : units::
      prefixed_unit<millikelvin, units::isq::si::milli, units::isq::si::kelvin>
{};

template<units::Representation Rep = std::int64_t>
using mK = units::isq::si::thermodynamic_temperature<millikelvin, Rep>;

/// Universal unit for temperature
using temperature = nK<std::int64_t>;
}  // namespace embed
