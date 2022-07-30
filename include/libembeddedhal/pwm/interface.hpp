#pragma once

#include <cstdint>

#include "../error.hpp"
#include "../frequency.hpp"
#include "../percent.hpp"

namespace hal {
/**
 * @addtogroup pwm
 * Available pwm APIs
 * @{
 */
/**
 * @brief Pulse Width Modulation (PWM) channel hardware abstraction.
 *
 */
class pwm
{
public:
  /// Generic settings for a hardware Pulse Width Modulation (PWM) generating
  /// devices devices.
  struct settings
  {
    /// The target channel PWM frequency.
    hal::frequency frequency = hal::frequency(1'000);

    /**
     * @brief Default operators for <, <=, >, >= and ==
     *
     * @return auto - result of the comparison
     */
    [[nodiscard]] constexpr auto operator<=>(const settings&) const noexcept =
      default;
  };

  /**
   * @brief Configure pwm to match the settings supplied
   *
   * @param p_settings - settings to apply to pwm driver
   * @return status
   * @throws std::errc::invalid_argument if the settings could not be achieved.
   */
  [[nodiscard]] status configure(const settings& p_settings) noexcept
  {
    return driver_configure(p_settings);
  }

  /**
   * @brief Set the duty cycle percentage
   *
   * @param p_duty_cycle - set the duty cycle of the pwm.
   * @return status - any error that occurred during this
   * operation.
   */
  [[nodiscard]] status duty_cycle(percent p_duty_cycle) noexcept
  {
    return driver_duty_cycle(p_duty_cycle);
  }

private:
  virtual status driver_configure(const settings& p_settings) noexcept = 0;
  virtual status driver_duty_cycle(percent p_duty_cycle) noexcept = 0;
};
/** @} */
}  // namespace hal
