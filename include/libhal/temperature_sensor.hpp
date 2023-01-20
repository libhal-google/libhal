#pragma once

#include "config.hpp"
#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief Temperature sensing hardware abstraction interface
 *
 *
 */
class temperature_sensor
{
public:
  struct read_t
  {
    /// Temperature measurement
    celsius temperature;
  };

  /**
   * @brief Read the current temperature measured by the device
   *
   * @return result<read_t> - temperature data
   */
  [[nodiscard]] result<read_t> read()
  {
    return driver_read();
  }

  virtual ~temperature_sensor() = default;

private:
  virtual result<read_t> driver_read() = 0;
};
}  // namespace hal
