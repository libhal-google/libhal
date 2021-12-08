#pragma once

#include <cinttypes>

#include "../driver.hpp"
#include "../full_scale.hpp"

namespace embed {
/**
 * @brief Analog to Digital Converter (ADC) hardware abstraction interface.
 *
 * Use this interface for devices and peripherals that can convert analog
 * voltage signals into a digital number.
 *
 */
class adc : public driver<>
{
  /**
   * @brief Read a sample from the analog to digital converter.
   *
   * This function makes no assumptions about how the conversation is made via
   * the ADC. Conversions could be performed at each call of this function.
   * Conversions can be ongoing by hardware and this function returns the latest
   * sample or the first sample from a queue. The conversation can also occur
   * over a communication protocol like SPI or I2C which could effect the time
   * it takes for this function to return. It is the responsibility of the
   * application developer to understand how a particular ADC will effect their
   * application.
   *
   * @return full_scale<uint32_t> Return the value of the ADC as a full_scale
   * value. Typical implemenation:
   * `return embed::bit_depth<uint32_t, 12>(adc_value);`.
   *
   */
  virtual full_scale<uint32_t> read() = 0;
};
}  // namespace embed
