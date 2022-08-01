#pragma once

#include "../config.hpp"
#include "../error.hpp"
#include "../percentage.hpp"

namespace hal {
/**
 * @addtogroup motor
 * Available motor APIs
 * @{
 */
/**
 * @brief Hardware abstraction for open loop continuous rotary and linear
 * actuators.
 *
 * The motor interface can represent a variety of things such as:
 *   - A driver for motor controller IC like the DRV8801
 *   - A driver for a motor with integrated controller & serial interface
 *   - A unidirectional motor controlled by a single transistor
 *   - A servo with open loop motor control
 *
 */
template<std::floating_point float_t = config::float_type>
class motor
{
public:
  /**
   * @brief Apply a percentage of power to the motor equal to the `p_power`
   * input parameter.
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
   * - Negative percentages will cause the motor to move in the opposite
   *   direction as positive percentages. In the event that motor driver can *
   *   only go in one direction, this function should clamp the power applied to
   *   0%.
   *
   * @param p_power - the amount of power to apply to the motor
   * @return status - success or an error that occurred when
   * attempting to set the power output of the motor.
   */
  [[nodiscard]] status power(percentage<float_t> p_power) noexcept
  {
    return driver_power(p_power);
  }

private:
  virtual status driver_power(percentage<float_t> p_power) noexcept = 0;
};
/** @} */
}  // namespace hal
