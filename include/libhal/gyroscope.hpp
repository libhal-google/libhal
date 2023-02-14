#pragma once

#include "config.hpp"
#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief Angular velocity sensing hardware abstraction interface.
 */
class gyroscope
{
public:
  /**
   * @brief Result from reading the gyroscope.
   *
   */
  struct read_t
  {
    /**
     * @brief Angular velocity in the X axis, relative to the device's reference
     * frame
     */
    rpm x;
    /**
     * @brief Angular velocity in the Y axis, relative to the device's reference
     * frame.
     *
     */
    rpm y;
    /**
     * @brief Angular velocity in the Z axis, relative to the device's reference
     * frame.
     *
     */
    rpm z;
  };

  /**
   * @brief Read the latest angular velocity sensed by the device
   *
   * @return result<read_t> - angular velocity data
   */
  [[nodiscard]] result<read_t> read()
  {
    return driver_read();
  }

  virtual ~gyroscope() = default;

private:
  virtual result<read_t> driver_read() = 0;
};
}  // namespace hal
