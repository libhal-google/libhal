#pragma once

#include <cstdint>

#include "../error.hpp"
#include "temperature_units.hpp"

namespace embed {
/**
 * @brief Hardware abstraction interface for temperature sensing devices.
 *
 */
class temperature_sensor
{
public:
  /**
   * @brief Read the current temperature from the sensor.
   *
   * @return boost::leaf::result<nanokelvin> - current temperature reading or an
   * error.
   */
  [[nodiscard]] boost::leaf::result<temperature> read() noexcept
  {
    return driver_read();
  }

private:
  virtual boost::leaf::result<temperature> driver_read() noexcept = 0;
};
}  // namespace embed
