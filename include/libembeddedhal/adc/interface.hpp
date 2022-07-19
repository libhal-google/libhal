#pragma once

#include <cstdint>

#include "../error.hpp"
#include "../percent.hpp"

namespace embed {
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
 *
 */
class adc_interface
{
public:
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
   * @return boost::leaf::result<percent> - Return the value of the
   * ADC as a full_scale value. Typical implementation for a 12-bit adc would
   * look like: `return embed::bit_depth<uint32_t, 12>(adc_value);`.
   *
   */
  [[nodiscard]] boost::leaf::result<percent> read() noexcept
  {
    return driver_read();
  }

private:
  virtual boost::leaf::result<percent> driver_read() noexcept = 0;
};

template<class T>
concept adc = std::is_base_of_v<embed::adc_interface, T>;

constexpr embed::adc_interface& to_interface(embed::adc auto& p_adc)
{
  return p_adc;
}
/** @} */
}  // namespace embed
