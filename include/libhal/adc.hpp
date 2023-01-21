#pragma once

#include "config.hpp"
#include "error.hpp"

namespace hal {
/**
 * @brief Analog to Digital Converter (ADC) hardware abstraction interface.
 *
 * Use this interface for devices and peripherals that can convert analog
 * voltage signals into a digital number.
 *
 * ADC peripheral only know the proportion of a voltage signal relative to a Vss
 * (negative reference) and a Vcc (positive reference) and thus cannot describe
 * the voltage directly.
 */
class adc
{
public:
  /**
   * @brief Sample the analog to digital converter and return the result
   *
   * @return result<float> - a value between 0.0f and +1.0f. The value
   * representing the voltage measured by the ADC from Vss to Vcc, where Vss is
   * the negative reference for the ADC and Vcc is the positive or supply
   * voltage for the ADC. For example, if Vss is 0V and Vcc is 5V and this
   * function returns 0.5f, then the voltage measured is 2.5V.
   */
  [[nodiscard]] result<float> read()
  {
    return driver_read();
  }

  virtual ~adc() = default;

private:
  virtual result<float> driver_read() = 0;
};
}  // namespace hal
