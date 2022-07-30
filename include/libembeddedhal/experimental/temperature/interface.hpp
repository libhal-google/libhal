#pragma once

#include <cstdint>

#include "../error.hpp"
#include "unit.hpp"

namespace hal {
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
  [[nodiscard]] boost::leaf::result<microkelvin> read() noexcept
  {
    return driver_read();
  }

private:
  virtual boost::leaf::result<microkelvin> driver_read() noexcept = 0;
};
}  // namespace hal
