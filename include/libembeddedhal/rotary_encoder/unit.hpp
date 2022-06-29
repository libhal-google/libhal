#pragma once

#include <cstdint>

#include <units/generic/angle.h>
#include <units/isq/si/prefixes.h>

namespace embed {
struct nanoradian
  : units::prefixed_unit<nanoradian, units::isq::si::nano, units::radian>
{};
template<units::Representation Rep = std::int64_t>
using nRad = units::angle<nanoradian, Rep>;

struct microradian
  : units::prefixed_unit<microradian, units::isq::si::micro, units::radian>
{};

template<units::Representation Rep = std::int64_t>
using uRad = units::angle<microradian, Rep>;

struct milliradian
  : units::prefixed_unit<milliradian, units::isq::si::milli, units::radian>
{};

template<units::Representation Rep = std::int64_t>
using mRad = units::angle<milliradian, Rep>;

/// Universal unit for angular position
using angular_position = nRad<std::int64_t>;
}  // namespace embed
