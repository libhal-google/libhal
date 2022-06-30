#pragma once

#include <units/isq/si/length.h>

namespace embed {
/// Universal unit for length
using length = units::isq::si::length<units::isq::si::nanometre, std::int64_t>;
}  // namespace embed
