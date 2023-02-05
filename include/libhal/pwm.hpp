#pragma once

#include <algorithm>
#include <cstdint>

#include "config.hpp"
#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief Pulse Width Modulation (PWM) channel hardware abstraction.
 *
 * This driver controls the waveform generation of a square wave and its
 * properties such as frequency and duty cycle.
 *
 * Frequency, meaning how often the waveform cycles from from low to high.
 *
 * Duty cycle, what proportion of the wavelength of the pulse is the voltage
 * HIGH.
 *
 *  ```
 *     ____________________       _
 *    |                    |     |
 *   _|                    |_____|
 *    ^                    ^     ^
 *    |<------ high ------>|<low>|
 *
 *    HIGH Duration = 18 segments
 *    LOW Duration =  5 segments
 *    Duty Cycle = 20 / (20 + 5) = 80%
 *
 *    If each segment is 1us then the wavelength is 25us
 *    Thus frequency is (1 / 25us) = 40kHz
 * ```
 *
 * PWM is used for power control like motor control, lighting, transmitting
 * signals to servos, sending telemetry and much more.
 *
 */
class pwm
{
public:
  /**
   * @brief Set the pwm waveform frequency
   *
   * @param p_frequency - settings to apply to pwm driver
   * @return status - success or failure
   * @throws std::errc::argument_out_of_domain - if the frequency is beyond what
   * the pwm generator is capable of achieving.
   */
  [[nodiscard]] status frequency(hertz p_frequency)
  {
    return driver_frequency(p_frequency);
  }

  /**
   * @brief Set the pwm waveform duty cycle
   *
   * The input value `p_duty_cycle` is a 32-bit floating point value from 0.0f
   * to 1.0f.
   *
   * The floating point value is directly proportional to the duty cycle
   * percentage, such that 0.0 is 0%, 0.25 is 25%, 0.445 is 44.5% and 1.0 is
   * 100%.
   *
   * This function clamps the input value between 0.0f and 1.0f and thus  values
   * passed to driver implementations are guaranteed to be within this range.
   * Callers of this function do not need to clamp their values before passing
   * them into this function as it would be redundant. The rationale for doing
   * this at the interface layer is that it allows callers and driver
   * implementors to omit redundant clamping code and thus reducing code bloat.
   *
   * @param p_duty_cycle - a value from 0.0 to 1.0 representing the duty cycle
   * percentage.
   * @return status - success or failure
   */
  [[nodiscard]] status duty_cycle(float p_duty_cycle)
  {
    auto clamped_duty_cycle = std::clamp(p_duty_cycle, 0.0f, 1.0f);
    return driver_duty_cycle(clamped_duty_cycle);
  }

  virtual ~pwm() = default;

private:
  virtual status driver_frequency(hertz p_frequency) = 0;
  virtual status driver_duty_cycle(float p_duty_cycle) = 0;
};
}  // namespace hal
