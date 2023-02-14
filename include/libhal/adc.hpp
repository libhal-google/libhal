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
   * @brief Result from reading the adc.
   *
   */
  struct read_t
  {
    /**
     * @brief Sample value of the adc
     *
     * Is guaranteed by the implementing driver to be between 0.0f and +1.0f.
     * The value representing the voltage measured by the ADC from Vss (negative
     * reference) to Vcc (positive reference). For example, if Vss is 0V (gnd)
     * and Vcc is 5V and this value is 0.5f, then the voltage measured is 2.5V.
     */
    float sample;
  };

  /**
   * @brief Sample the analog to digital converter and return the result
   *
   * @return result<read_t> - the sampled adc value
   */
  [[nodiscard]] result<read_t> read()
  {
    return driver_read();
  }

  virtual ~adc() = default;

private:
  virtual result<read_t> driver_read() = 0;
};
}  // namespace hal
