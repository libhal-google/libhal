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
 * @brief Digital output pin hardware abstraction.
 *
 * Use this to drive a pin HIGH or LOW in order to send a control signal or turn
 * off or on an LED.
 *
 * Implementations of this interface can be backed by external devices such as
 * I/O expanders or other micro-controllers.
 *
 */
class output_pin
{
public:
  /// Generic settings for output pins
  struct settings
  {
    /// Pull resistor for the pin. This generally only helpful when open
    /// drain is enabled.
    pin_resistor resistor = pin_resistor::none;

    /// Starting level of the output pin. HIGH voltage defined as true and LOW
    /// voltage defined as false.
    bool open_drain = false;
  };

  /**
   * @brief Feedback from setting the pin state
   *
   * This structure is currently empty as no feedback has been determined for
   * now. This structure may be expanded in the future.
   */
  struct set_level_t
  {};

  /// Pin level reading structure
  struct level_t
  {
    /// Current state of the pin
    bool state;
  };

  /**
   * @brief Configure the output pin to match the settings supplied
   *
   * @param p_settings - settings to apply to output pin
   * @return status - success or failure
   * @throws std::errc::invalid_argument if the settings could not be achieved.
   */
  [[nodiscard]] status configure(const settings& p_settings)
  {
    return driver_configure(p_settings);
  }

  /**
   * @brief Set the state of the pin
   *
   * @param p_high - if true then the pin state is set to HIGH voltage. If
   * false, the pin state is set to LOW voltage.
   * @return result<set_level_t> - success or failure
   */
  [[nodiscard]] result<set_level_t> level(bool p_high)
  {
    return driver_level(p_high);
  }

  /**
   * @brief Read the current state of the output pin
   *
   * Implementations must read the pin state from hardware and will not simply
   * cache the results from the execution of `level(bool)`.
   *
   * This pin may not equal the state set by `level(bool)` when the pin is
   * configured as open-drain.
   *
   * @return result<level_t> - return the current level state of the output pin
   */
  [[nodiscard]] result<level_t> level()
  {
    return driver_level();
  }

  virtual ~output_pin() = default;

private:
  virtual status driver_configure(const settings& p_settings) = 0;
  virtual result<set_level_t> driver_level(bool p_high) = 0;
  virtual result<level_t> driver_level() = 0;
};
}  // namespace hal
