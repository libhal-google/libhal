#pragma once

#include <cinttypes>

#include "../driver.hpp"
#include "../full_scale.hpp"

namespace embed {
/**
 * @brief Digital to Analog Converter (DAC) hardware abstraction interface.
 *
 * Use this interface for devices and peripherals that can create arbitrary
 * analog voltages between a defined LOW and HIGH voltage.
 *
 */
class dac : public driver<>
{
public:
  /**
   * @brief Generate a voltage between a defined LOW and HIGH voltage.
   *
   * The LOW and HIGH voltage are generally configured by a reference voltage
   * (usually denoted Vref) connected to the device. Generally though, the low
   * voltage is 0V and the high voltage reference is VCC.
   *
   * @param value precentage scale from LOW to HIGH
   */
  virtual void write(full_scale<uint32_t> value) = 0;
};
} // namespace embed
