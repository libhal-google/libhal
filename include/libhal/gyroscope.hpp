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
   * @return read_t - angular velocity data
   */
  [[nodiscard]] read_t read()
  {
    return driver_read();
  }

  virtual ~gyroscope() = default;

private:
  virtual read_t driver_read() = 0;
};
}  // namespace hal
