#pragma once

#include <cstdint>

#include "../error.hpp"
#include "../percent.hpp"

namespace hal {
/**
 * @addtogroup dac
 * Available Digital to Analog Converter (DAC) APIs
 * @{
 */
/**
 * @brief Digital to Analog Converter (DAC) hardware abstraction interface.
 *
 * Use this interface for devices and peripherals that can create arbitrary
 * analog voltages between a defined LOW and HIGH voltage.
 *
 */
class dac
{
public:
  /**
   * @brief Generate a voltage between a defined LOW and HIGH voltage.
   *
   * The LOW and HIGH voltage are generally configured by a reference voltage
   * (usually denoted V_ref) connected to the device. Generally though, the low
   * voltage is 0V and the high voltage reference is VCC.
   *
   * @param p_value - percentage scale from LOW to HIGH voltage
   * @return status - any error that occurred during this
   * operation.
   */
  [[nodiscard]] status write(percent p_value) noexcept
  {
    return driver_write(p_value);
  }

private:
  virtual status driver_write(percent p_value) noexcept = 0;
};
/** @} */
}  // namespace hal
