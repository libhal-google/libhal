#pragma once

#include <cinttypes>

#include "../driver.hpp"
#include "../full_scale.hpp"

namespace embed {
/**
 * @brief Analog to Digital Converter (ADC) hardware abstraction interface. Use
 * this interface for devices and peripherals that can convert analog voltage
 * signals into a digital number.
 *
 */
class adc : public driver<>
{
  /**
   * @brief Read a sample from the analog to digital converter. This function
   * makes no assumptions about how the conversation is made via the ADC.
   * Conversions could be performed at each call of this function. Conversions
   * can be ongoing by hardware and this function returns the latest sample or
   * the first sample from a queue. The conversation can also occur over a
   * communication protocol like SPI or I2C which could effect the time it takes
   * for this function to return. It is the responsibility of the application
   * developer to understand how a particular ADC will effect their application.
   *
   * @return full_scale<uint32_t> This value acts a precentage scale from where
   * 0 represents the lowest possible voltage reading the ADC can return and
   * UINT32T_MAX representing the maximum. It is up to the application developer
   * to know what the low and high voltage ranges are for the ADC and scale this
   * value to the correct voltage as needed. As an example, if the low end for
   * the ADC is 0V and the high is 3.3V and the input voltage is 1.65V (or
   * half 3.3V), then the full scale value will return 2147483647 or (2^32 - 1)
   * / 2.
   */
  virtual full_scale<uint32_t> read() = 0;
};
} // namespace embed
