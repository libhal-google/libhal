#pragma once

#include "../driver.hpp"
#include "../full_scale.hpp"

#include <cinttypes>

namespace embed {
/**
 * @brief Pulse Width Modulation (PWM) channel hardware abstraction.
 *
 */
class pwm : public driver<>
{
  /**
   * @brief Set the operating frequency of the pwm channel. Note that on many
   * implementations, setting the frequency of a singular channel can have the
   * effect of changing the frequency of all of the channels on the same pwm
   * peripheral.
   *
   * @param p_frequency_hz frequency of the pwm channel in hertz
   */
  virtual void frequency(uint32_t p_frequency_hz) = 0;
  /**
   * @brief Get the operating frequency of the pwm channel
   *
   * @return uint32_t frequency of the pwm channel in hertz
   */
  virtual uint32_t frequency() = 0;
  /**
   * @brief Set the duty cycle percentage
   *
   * @param p_duty_cycle set the duty cycle of the pwm.
   */
  virtual void duty_cycle(full_scale<uint32_t> p_duty_cycle) = 0;
  /**
   * @brief Get the duty cycle percentage
   *
   * @return full_scale<uint32_t> duty cycle
   */
  virtual full_scale<uint32_t> duty_cycle() = 0;
};
}  // namespace embed
