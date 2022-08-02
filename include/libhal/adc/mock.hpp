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
template<std::floating_point float_t = config::float_type>
struct adc : public hal::adc<float_t>
{
  /**
   * @brief Construct a new adc object
   *
   * @param p_adc_value - percent value for adc
   */
  adc(percentage<float_t> p_adc_value)
    : m_adc_value(p_adc_value){};
  /**
   * @brief Set the mock adc to a given value
   *
   * @param p_adc_value - percent value to set adc to
   */
  void set(percentage<float_t> p_adc_value)
  {
    m_adc_value = p_adc_value;
  }

private:
  result<percentage<float_t>> driver_read() noexcept
  {
    return m_adc_value;
  }

  percentage<float_t> m_adc_value = percentage<float_t>(0.0);
};
/** @} */
}  // namespace hal::mock
