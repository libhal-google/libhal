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
 * @brief angular velocity sensor hardware abstraction interface
 *
 */
class angular_velocity_sensor
{
public:
  /**
   * @brief angular velocity reading from the sensor
   *
   *
   */
  struct read_t
  {
    /**
     * @brief The angular velocity in rotations per minute. (Underlying type is
     * an std::float)
     *
     */
    hal::rpm angular_velocity = 0;
  };
  /**
   * @brief Reads the most up to date angular velocity from the sensor
   *
   * @return result<read_t> - angular velocity data
   */
  [[nodiscard]] result<read_t> read()
  {
    return driver_read();
  }

  virtual ~angular_velocity_sensor() = default;

private:
  virtual hal::result<read_t> driver_read() = 0;
};
}  // namespace hal
