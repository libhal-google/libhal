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

  /**
   * @brief Read the current temperature from the sensor.
   *
   * @param p_sample -
   * @return boost::leaf::result<std::span<temperature>> - a span with length
   * based on the number of
   */
  [[nodiscard]] boost::leaf::result<std::span<temperature>> read(
    std::span<temperature> p_sample) noexcept
  {
    return driver_read(p_sample);
  }

private:
  virtual boost::leaf::result<std::span<temperature>> driver_read(
    std::span<temperature> p_sample) noexcept = 0;
};
}  // namespace embed
