#pragma once

#include "../error.hpp"
#include "../frequency.hpp"
#include "../percent.hpp"

#include <cstdint>

namespace embed {
/// Pulse Width Modulation (PWM) channel hardware abstraction.
class pwm
{
public:
  /// Generic settings for a hardware Pulse Width Modulation (PWM) generating
  /// devices devices.
  struct settings
  {
    /// The target channel PWM frequency.
    embed::frequency frequency = embed::frequency(1'000);
  };

  /**
   * @brief Configure pwm to match the settings supplied
   *
   * @param p_settings - settings to apply to pwm driver
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation. Will return embed::error::invalid_settings if the settings could
   * not be achieved.
   */
  [[nodiscard]] boost::leaf::result<void> configure(
    const settings& p_settings) noexcept
  {
    return driver_configure(p_settings);
  }

  /**
   * @brief Set the duty cycle percentage
   *
   * @param p_duty_cycle - set the duty cycle of the pwm.
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  [[nodiscard]] boost::leaf::result<void> duty_cycle(
    percent p_duty_cycle) noexcept
  {
    return driver_duty_cycle(p_duty_cycle);
  }

private:
  virtual boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept = 0;
  virtual boost::leaf::result<void> driver_duty_cycle(
    percent p_duty_cycle) noexcept = 0;
};
}  // namespace embed
