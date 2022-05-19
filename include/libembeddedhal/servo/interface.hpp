#pragma once

#include "../error.hpp"
#include "unit.hpp"

namespace embed {
/**
 * @brief Hardware abstraction for a closed loop positional rotary actuator.
 *
 * The servo interface represents any sort of rotation position controlled
 * actuator regardless of what it uses for feedback or for an actuator
 *
 */
class servo
{
public:
  struct bounds_t
  {
    embed::angle minimum;
    embed::angle maximum;
  };

  boost::leaf::result<bounds_t> bounds() {}

  boost::leaf::result<void> angle(embed::angle p_angle)
  {
    return driver_angle(p_angle);
  }

private:
  virtual boost::leaf::result<void> driver_angle(embed::angle p_angle) = 0;
};
}  // namespace embed
