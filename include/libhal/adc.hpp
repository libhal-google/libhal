#pragma once

#include <cstdint>

#include "config.hpp"
#include "error.hpp"

namespace hal {
/**
 * @addtogroup adc
 * Available Analog to Digital Converter (ADC) APIs
 * @{
 */
/**
 * @brief Analog to Digital Converter (ADC) hardware abstraction interface.
 *
 * Use this interface for devices and peripherals that can convert analog
 * voltage signals into a digital number.
 */
class adc
{
public:
  /**
   * @brief Read a sample from the analog to digital converter.
   *
   * @return result<float> - float of the voltage from Vss to Vcc, where
   * Vss is the negative or ground reference for the ADC and Vcc is the positive
   * or supply voltage for the ADC. For example with a Vss of 0V and Vcc of 4V,
   * a reading of 0.5 would represent 2V.
   */
  [[nodiscard]] result<float> read() noexcept
  {
    return driver_read();
  }

private:
  virtual result<float> driver_read() noexcept = 0;
};
/** @} */
}  // namespace hal
