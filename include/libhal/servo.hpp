#pragma once

#include "config.hpp"
#include "error.hpp"

namespace hal {
/**
 * @brief Hardware abstraction for closed loop positional rotary and linear
 * actuators.
 *
 * The servo interface can represent a variety of things such as:
 *   - Hobby RC Servos (RC = Radio Controlled)
 *   - Industrial servo controllers
 *   - Motors combined with a position feedback mechanism relative or absolute
 *   - Position controlled linear actuators
 *
 * The servo interface can represent absolute and relative position servos.
 * The meaning of these is defined as follows:
 *
 *   - Relative position servos, at power on, cannot determine their position
 *     and thus start at position angle 0. Generally, if a relative position
 *     servo is powered on and it commanded to move to position 0, meaning its
 *     center position it will not move. With a 180 degree rotary servo, if it
 *     is commanded to move to 50%, this corresponds to 45 degrees to the
 *     right, and thus the motor will move 45 degrees to the right relative to
 *     where it started.
 *   - Absolute position servos, at power on, can determine the position of
 *     their output shaft relative to their servo housing. When these servos are
 *     commanded to move to their center position, they will be able to move to
 *     that position no matter the original position of the rotor at power on.
 */
class servo
{
public:
  /**
   * @brief Set the position of the servo.
   *
   * The position of the servo is controlled with a float value. The
   * percentage value is then converted into an angular or linear position by
   * the implementing driver. The following list gives an idea of how each
   * percentage corresponds to each position.
   *
   * - position = 0%: corresponds to the center position of the servo. Relative
   *   servos start at this position and will not move if commanded to this
   *   position at start with no external movement of the shaft. Absolute servos
   *   will move their rotor shaft to this position if commanded and the motor
   *   shaft is not already at this position.
   *
   * - position = 100%: corresponds to the maximum positive position of the
   *   motor which is in the clockwise position for rotary servos and to the
   *   right for linear servos.
   *
   * - position = 50%: corresponds to the midway point between the maximum
   *   position of the motor and the center position.
   *
   * - position = -100%: corresponds to the maximum negative position of the
   *   motor in the counterclockwise position for rotary servos and to the left
   *   for linear servos.
   *
   * The range of a servo, in units of rotation or length, can vary from servo
   * to servo and is intrinsic to the servo itself.
   *
   * USAGE:
   *
   *     // This is a made up servo has 180 degrees of movement and has absolute
   *     // positioning.
   *     hal::example_servo servo;
   *     // Move to center position
   *     servo.position(hal::float(0.0/90.0));
   *     // ... delay ...
   *     // Move to the +45 degrees position
   *     servo.position(hal::float(45.0/90.0));
   *     // ... delay ...
   *     // Move to the -45 degrees position
   *     servo.position(hal::float(-45.0/90.0));
   *     // ... delay ...
   *     // Move to the -90 degrees position
   *     servo.position(hal::float(-90.0/90.0));
   *     // ... delay ...
   *     // Move to the +90 degrees position
   *     servo.position(hal::float(90.0/90.0));
   *
   * @param p_position - position to move the servo to
   * @return status - success or failure
   */
  [[nodiscard]] status position(float p_position) noexcept
  {
    return driver_position(p_position);
  }

  virtual ~servo() = default;

private:
  virtual status driver_position(float p_position) noexcept = 0;
};
}  // namespace hal
