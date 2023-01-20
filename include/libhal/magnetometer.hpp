#pragma once

#include "config.hpp"
#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief Magnetic field strength sensing hardware abstraction interface.
 *
 * Magnetometers are usually used for determining the strength of a magnetic
 * field, or calculating compass headings. If the device that the magnetometer
 * is mounted on, tends to move, or change its own orientation, then it is
 * helpful to use an accelerometer or tilt sensor in order to determine
 * appropriate heading for compass calculations.
 */
class magnetometer
{
public:
  struct read_t
  {
    /// Magnetic field strength in the X axis, relative to the device's
    /// reference frame.
    gauss x;
    /// Magnetic field strength in the X axis, relative to the device's
    /// reference frame.
    gauss y;
    /// Magnetic field strength in the X axis, relative to the device's
    /// reference frame.
    gauss z;
  };

  /**
   * @brief Read the latest magnetic field strength sensed by the device
   *
   * @return result<read_t> - magnetic field strength data
   */
  [[nodiscard]] result<read_t> read()
  {
    return driver_read();
  }

  virtual ~magnetometer() = default;

private:
  virtual result<read_t> driver_read() = 0;
};
}  // namespace hal
