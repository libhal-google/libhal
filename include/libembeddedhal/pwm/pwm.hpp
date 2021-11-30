#pragma once

#include "../driver.hpp"
#include "../full_scale.hpp"

#include <cinttypes>

namespace embed {

class pwm : public driver<>
{
  /// @brief Set the operating frequency of the PWM channel
  /// @param frequency_hz frequency of the PWM channel in hertz
  virtual void frequency(uint32_t frequency_hz) = 0;

  /// @brief Get the operating frequency of the PWM channel
  /// @return frequency of the PWM channel in hertz
  virtual uint32_t frequency() = 0;

  /// Set the duty cycle of the waveform
  ///
  /// @param duty_cycle set the duty cycle of the pwm. 0% == 0x0000'0000 and
  /// 100% is 0xFFFF'FFFF.
  virtual void duty_cycle(full_scale<uint32_t> duty_cycle) = 0;

  /// Get the duty cycle percentage
  ///
  /// @return duty cycle from 0x0000'0000 to 0xFFFF'FFFF.
  virtual full_scale<uint32_t> duty_cycle() = 0;
};
}
