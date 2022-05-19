#pragma once

#include <cstdint>

#include <units/generic/angle.h>
#include <units/isq/si/prefixes.h>

// TODO(#242): replace radians with degrees

namespace embed {

struct nanorad
  : units::prefixed_unit<nanorad, units::isq::si::nano, units::radian>
{};

template<units::Representation Rep = std::int64_t>
using nrad = units::angle<nanorad, Rep>;

struct microrad
  : units::prefixed_unit<microrad, units::isq::si::micro, units::radian>
{};

template<units::Representation Rep = std::int64_t>
using urad = units::angle<microrad, Rep>;

struct millirad
  : units::prefixed_unit<millirad, units::isq::si::milli, units::radian>
{};

template<units::Representation Rep = std::int64_t>
using mrad = units::angle<millirad, Rep>;

using angle = nrad<std::int64_t>;
}  // namespace embed
