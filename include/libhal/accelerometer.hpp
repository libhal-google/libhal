#pragma once

#include "config.hpp"
#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief Acceleration sensing hardware abstraction interface.
 */
class accelerometer
{
public:
  struct read_t
  {
    /// Acceleration in the X axis, relative to the device's reference frame.
    g_force x;
    /// Acceleration in the Y axis, relative to the device's reference frame.
    g_force y;
    /// Acceleration in the Z axis, relative to the device's reference frame.
    g_force z;
  };

  /**
   * @brief Read the latest acceleration sensed by the device
   *
   * @return result<read_t> - acceleration data
   */
  [[nodiscard]] result<read_t> read()
  {
    return driver_read();
  }

  virtual ~accelerometer() = default;

private:
  virtual result<read_t> driver_read() = 0;
};
}  // namespace hal
