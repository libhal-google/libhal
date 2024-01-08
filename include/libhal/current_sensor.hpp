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
 * @brief current sensor hardware abstraction interface
 *
 */
class current_sensor
{
public:
  /**
   * @brief current reading from the sensor
   *
   *
   */
  struct read_t
  {
    hal::ampere current = 0.0f;
  };
  /**
   * @brief Reads the most up to date current from the sensor
   *
   * @return result<read_t> - current data
   */
  [[nodiscard]] result<read_t> read()
  {
    return driver_read();
  }

  virtual ~current_sensor() = default;

private:
  virtual hal::result<read_t> driver_read() = 0;
};
}  // namespace hal
