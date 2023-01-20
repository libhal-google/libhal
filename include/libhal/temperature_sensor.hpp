#pragma once

#include "config.hpp"
#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief
 */
class temperature_sensor
{
public:
  struct read_t
  {
    celsius temperature;
  };

  /**
   * @brief
   *
   * @return result<read_t> -
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
