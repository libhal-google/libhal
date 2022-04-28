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

template<units::Representation Rep = std::int64_t>
using nm_per_s2 =
  units::isq::si::acceleration<embed::nanometre_per_second_sq, Rep>;

struct micrometre_per_second_sq
  : units::derived_unit<micrometre_per_second_sq,
                        units::isq::si::dim_acceleration,
                        units::isq::si::micrometre,
                        units::isq::si::second>
{};

template<units::Representation Rep = std::int64_t>
using um_per_s2 =
  units::isq::si::acceleration<embed::micrometre_per_second_sq, Rep>;

struct millimetre_per_second_sq
  : units::derived_unit<millimetre_per_second_sq,
                        units::isq::si::dim_acceleration,
                        units::isq::si::millimetre,
                        units::isq::si::second>
{};

template<units::Representation Rep = std::int64_t>
using mm_per_s2 =
  units::isq::si::acceleration<embed::millimetre_per_second_sq, Rep>;

/// Universal unit for acceleration
using acceleration = nm_per_s2<std::int64_t>;
}  // namespace embed
