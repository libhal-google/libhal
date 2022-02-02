#pragma once

#include "../driver.hpp"
#include "../frequency.hpp"
#include "../percent.hpp"

#include <cinttypes>

namespace embed {
/// Generic settings for a hardware Pulse Width Modulation (PWM) generating
/// devices devices.
struct pwm_settings
{
  /// The target channel PWM frequency.
  embed::frequency frequency = embed::frequency(1'000);
};
/**
 * @brief Pulse Width Modulation (PWM) channel hardware abstraction.
 *
 */
class pwm : public driver<pwm_settings>
{
  /**
   * @brief Set the duty cycle percentage
   *
   * @param p_duty_cycle - set the duty cycle of the pwm.
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  virtual boost::leaf::result<void> duty_cycle(percent p_duty_cycle) = 0;
};
}  // namespace embed
