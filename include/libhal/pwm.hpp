#pragma once

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
  [[nodiscard]] status frequency(hertz p_frequency) noexcept
  {
    return driver_frequency(p_frequency);
  }

  /**
   * @brief Set the pwm waveform duty cycle
   *
   * @param p_duty_cycle - set the duty cycle of the pwm
   * @return status - success or failure
   */
  [[nodiscard]] status duty_cycle(float p_duty_cycle) noexcept
  {
    return driver_duty_cycle(p_duty_cycle);
  }

  virtual ~pwm() = default;

private:
  virtual status driver_frequency(hertz p_frequency) noexcept = 0;
  virtual status driver_duty_cycle(float p_duty_cycle) noexcept = 0;
};
}  // namespace hal
