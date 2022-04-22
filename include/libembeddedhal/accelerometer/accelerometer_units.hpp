#pragma once

#include <units/isq/si/acceleration.h>
#include <units/isq/si/prefixes.h>

namespace embed {
struct nanometre_per_second_sq
  : units::derived_unit<nanometre_per_second_sq,
                        units::isq::si::dim_acceleration,
                        units::isq::si::nanometre,
                        units::isq::si::second>
{};
struct micrometre_per_second_sq
  : units::derived_unit<micrometre_per_second_sq,
                        units::isq::si::dim_acceleration,
                        units::isq::si::micrometre,
                        units::isq::si::second>
{};
struct millimetre_per_second_sq
  : units::derived_unit<millimetre_per_second_sq,
                        units::isq::si::dim_acceleration,
                        units::isq::si::millimetre,
                        units::isq::si::second>
{};
struct kilometre_per_second_sq
  : units::derived_unit<kilometre_per_second_sq,
                        units::isq::si::dim_acceleration,
                        units::isq::si::kilometre,
                        units::isq::si::second>
{};
}  // namespace embed
