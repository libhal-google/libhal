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
 * @brief Digital input pin hardware abstraction interface.
 *
 * Use this to read a pin and determine if the voltage on it is HIGH or LOW.
 *
 */
class input_pin
{
public:
  /// Generic settings for input pins
  struct settings
  {
    /// Pull resistor for an input pin
    pin_resistor resistor = pin_resistor::pull_up;
  };

  /// Input pin level reading structure
  struct level_t
  {
    /// Measured state of the pin
    bool state;
  };

  /**
   * @brief Configure the input pin to match the settings supplied
   *
   * @param p_settings - settings to apply to input pin
   * @throws std::errc::invalid_argument if the settings could not be
   * achieved.
   */
  void configure(const settings& p_settings)
  {
    return driver_configure(p_settings);
  }

  /**
   * @brief Read the state of the input pin
   *
   * @return bool - true indicates HIGH voltage level and false
   * indicates LOW voltage level
   */
  [[nodiscard]] level_t level()
  {
    return driver_level();
  }

  virtual ~input_pin() = default;

private:
  virtual void driver_configure(const settings& p_settings) = 0;
  virtual level_t driver_level() = 0;
};
}  // namespace hal
