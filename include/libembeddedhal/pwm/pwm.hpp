#pragma once

#include "../driver.hpp"
#include "../frequency.hpp"
#include "../percent.hpp"

#include <cinttypes>

namespace embed {
/**
 * @brief Pulse Width Modulation (PWM) channel hardware abstraction.
 *
 */
class pwm : public driver<>
{
  /// default constructor
  pwm() = default;
  /// Explicitly delete copy constructor to prevent slicing
  pwm(const pwm& p_other) = delete;
  /// Explicitly delete assignment operator to prevent slicing
  pwm& operator=(const pwm& p_other) = delete;
  /// Destroy the object
  virtual ~pwm() = default;

  /**
   * @brief Set the operating frequency of the pwm channel. Note that on many
   * implementations, setting the frequency of a singular channel can have the
   * effect of changing the frequency of all of the channels on the same pwm
   * peripheral.
   *
   * @param p_frequency - frequency of the pwm channel in hertz
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  virtual boost::leaf::result<void> frequency(embed::frequency p_frequency) = 0;
  /**
   * @brief Get the operating frequency of the pwm channel
   *
   * @return boost::leaf::result<embed::frequency> - frequency of the pwm
   * channel in hertz
   */
  virtual boost::leaf::result<embed::frequency> frequency() = 0;
  /**
   * @brief Set the duty cycle percentage
   *
   * @param p_duty_cycle - set the duty cycle of the pwm.
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  virtual boost::leaf::result<void> duty_cycle(percent p_duty_cycle) = 0;
  /**
   * @brief Get the duty cycle percentage
   *
   * @return boost::leaf::result<percent> - duty cycle
   */
  virtual percent duty_cycle() = 0;
};
}  // namespace embed
