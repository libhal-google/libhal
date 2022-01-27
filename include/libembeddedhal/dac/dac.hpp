#pragma once

#include <cinttypes>

#include "../driver.hpp"
#include "../percent.hpp"

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
  /// default constructor
  dac() = default;
  /// Explicitly delete copy constructor to prevent slicing
  dac(const dac& p_other) = delete;
  /// Explicitly delete assignment operator to prevent slicing
  dac& operator=(const dac& p_other) = delete;
  /// Destroy the object
  virtual ~dac() = default;

  /**
   * @brief Generate a voltage between a defined LOW and HIGH voltage.
   *
   * The LOW and HIGH voltage are generally configured by a reference voltage
   * (usually denoted V_ref) connected to the device. Generally though, the low
   * voltage is 0V and the high voltage reference is VCC.
   *
   * @param p_value - precentage scale from LOW to HIGH
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  virtual boost::leaf::result<void> write(percent p_value) = 0;
};
}  // namespace embed
