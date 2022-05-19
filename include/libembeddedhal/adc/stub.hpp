#pragma once

#include "interface.hpp"

namespace embed::stub {
/**
 * @brief A stub implementation of embed::adc to be used as a placeholder for
 * drivers that require an optional adc. Calling read() will return the percent
 * variable that is configured in the constructor. This implementaion should
 * only be used when a driver has stated in its documentation that the adc is
 * optional and can be stubbed out.
 *
 */
class adc : public embed::adc
{
public:
  /**
   * @brief Construct a new adc object
   *
   * @param p_adc_value - percent value for adc
   */
  adc(embed::percent p_adc_value)
    : m_adc_value(p_adc_value){};

private:
  boost::leaf::result<percent> driver_read() noexcept { return m_adc_value; }

  embed::percent m_adc_value = embed::percent::from_ratio(0, 1);
};
}  // namespace embed::stub
