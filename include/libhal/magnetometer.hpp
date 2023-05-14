// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

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
  /**
   * @brief Result from reading the accelerometer.
   *
   */
  struct read_t
  {
    /**
     * @brief Magnetic field strength in the X axis, relative to the device's
     * reference frame.
     *
     */
    gauss x;

    /**
     * @brief Magnetic field strength in the Y axis, relative to the device's
     * reference frame.
     *
     */
    gauss y;

    /**
     * @brief Magnetic field strength in the Z axis, relative to the device's
     * reference frame.
     *
     */
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
