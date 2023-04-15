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
  /**
   * @brief Result from reading the temperature sensor.
   *
   */
  struct read_t
  {
    /**
     * @brief Measured temperature
     *
     */
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
