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
   * @brief Feedback setting the pwm duty cycle.
   *
   * This structure is currently empty as no feedback has been determined for
   * now. This structure may be expanded in the future.
   */
  struct duty_cycle_t
  {};

  /**
   * @brief Feedback setting the pwm frequency.
   *
   * This structure is currently empty as no feedback has been determined for
   * now. This structure may be expanded in the future.
   */
  struct frequency_t
  {};

  /**
   * @brief Set the pwm waveform frequency
   *
   * This function clamps the input value between 1.0_Hz and 1.0_GHz and thus
   * values passed to driver implementations are guaranteed to be within this
   * range. Callers of this function do not need to clamp their values before
   * passing them into this function as it would be redundant. The rationale for
   * doing this at the interface layer is that it allows callers and driver
   * implementors to omit redundant clamping code, reducing code bloat.
   *
   * @param p_frequency - settings to apply to pwm driver
   * @return result<frequency_t> - success or failure
   * @throws std::errc::argument_out_of_domain - if the frequency is beyond what
   * the pwm generator is capable of achieving.
   */
  [[nodiscard]] result<frequency_t> frequency(hertz p_frequency)
  {
    auto clamped_frequency = std::clamp(p_frequency, 1.0_Hz, 1.0_GHz);
    return driver_frequency(clamped_frequency);
  }

  /**
   * @brief Set the pwm waveform duty cycle
   *
   * The input value `p_duty_cycle` is a 32-bit floating point value from 0.0f
   * to 1.0f.
   *
   * The floating point value is directly proportional to the duty cycle
   * percentage, such that 0.0f is 0%, 0.25f is 25%, 0.445f is 44.5% and 1.0f is
   * 100%.
   *
   * This function clamps the input value between 0.0f and 1.0f and thus  values
   * passed to driver implementations are guaranteed to be within this range.
   * Callers of this function do not need to clamp their values before passing
   * them into this function as it would be redundant. The rationale for doing
   * this at the interface layer is that it allows callers and driver
   * implementors to omit redundant clamping code, reducing code bloat.
   *
   * @param p_duty_cycle - a value from 0.0f to +1.0f representing the duty
   * cycle percentage.
   * @return result<duty_cycle_t> - success or failure
   */
  [[nodiscard]] result<duty_cycle_t> duty_cycle(float p_duty_cycle)
  {
    auto clamped_duty_cycle = std::clamp(p_duty_cycle, 0.0f, 1.0f);
    return driver_duty_cycle(clamped_duty_cycle);
  }

  virtual ~pwm() = default;

private:
  virtual result<frequency_t> driver_frequency(hertz p_frequency) = 0;
  virtual result<duty_cycle_t> driver_duty_cycle(float p_duty_cycle) = 0;
};
}  // namespace hal
