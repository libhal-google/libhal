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
  /**
   * @brief Result from reading the accelerometer.
   *
   */
  struct read_t
  {
    /**
     * @brief Acceleration in the X axis, relative to the device's reference
     * frame.
     *
     */
    g_force x;
    /**
     * @brief Acceleration in the Y axis, relative to the device's reference
     * frame.
     *
     */
    g_force y;
    /**
     * @brief Acceleration in the Z axis, relative to the device's reference
     * frame.
     *
     */
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
