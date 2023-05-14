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

namespace hal {
/**
 * @brief Hardware abstraction for an open loop rotational actuator
 *
 * The motor interface can represent a variety of things such as:
 *
 *   - A driver for motor controller IC like the DRV8801
 *   - A driver for a motor with integrated controller & serial interface
 *   - A unidirectional motor controlled by a single transistor
 *   - A servo with open loop motor control
 *
 */
class motor
{
public:
  /**
   * @brief Feedback from setting the motor power
   *
   * This structure is currently empty as no feedback has been determined for
   * now. This structure may be expanded in the future.
   */
  struct power_t
  {};

  /**
   * @brief Apply power to the motor
   *
   * Power is a percentage and thus cannot be used as a way to gauge how fast
   * the motor is moving. In general applying more power means to increase speed
   * and/or torque to the motor.
   *
   * - 0% power would mean that no power is being applied to the motor. In this
   *   situation an unloaded motor will not move. 0% power does not guarantee
   *   that the motor will hold its position. These specifics depend greatly on
   *   the type of motor used and careful selection of motor and motor driver
   *   are important for applications using this interface.
   *
   * - 100% power means that the maximum available of power is being applied to
   *   the motor. As an example, if the max voltage of a DC brushed motor's
   *   power supply is 12V, then 12V would be supplied to this motor.
   *
   * - 50% power would mean that half of the available power is being applied to
   *   the motor. Using the same example, in this case 6V would be applied to
   *   the motor either as a DC constant voltage or via PWM at 50% duty cycle.
   *
   * - Negative values will cause the motor to move in the opposite
   *   direction as positive values. In the event that motor driver can *
   *   only go in one direction, this function should clamp the power applied to
   *   0%.
   *
   * @param p_power - Percentage of power to apply to the motor from -1.0f to
   * +1.0f, -100% to 100%, respectively.
   * @return result<power_t> - success or failure
   */
  [[nodiscard]] result<power_t> power(float p_power)
  {
    auto clamped_power = std::clamp(p_power, -1.0f, +1.0f);
    return driver_power(clamped_power);
  }

  virtual ~motor() = default;

private:
  virtual result<power_t> driver_power(float p_power) = 0;
};
}  // namespace hal
