#pragma once
#include "interface.hpp"

namespace hal::mock {
/**
 * @addtogroup adc
 * @{
 */
/**
 * @brief Mock adc implementation for use in unit tests and simulations.
 */
struct adc : public hal::adc
{
  /**
   * @brief Construct a new adc object
   *
   * @param p_adc_value - percent value for adc
   */
  adc(hal::percent p_adc_value)
    : m_adc_value(p_adc_value){};
  /**
   * @brief Set the mock adc to a given value
   *
   * @param p_adc_value - percent value to set adc to
   */
  void set(hal::percent p_adc_value)
  {
    m_adc_value = p_adc_value;
  }

private:
  result<percent> driver_read() noexcept
  {
    return m_adc_value;
  }

  hal::percent m_adc_value = hal::percent::from_ratio(0, 1);
};
/** @} */
}  // namespace hal::mock
