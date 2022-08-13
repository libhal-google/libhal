#pragma once

#include <queue>

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
   * @brief Queues the percentages to be returned for read()
   *
   * @param p_adc_values - queue of percentages
   */
  void set(std::queue<percentage>& p_adc_values)
  {
    m_adc_values = p_adc_values;
  }

private:
  result<percentage> driver_read() noexcept
  {
    if (m_adc_values.size() == 0) {
      return boost::leaf::new_error(
        std::out_of_range("percentages queue is empty!"));
    }
    percentage m_current_value = m_adc_values.front();
    m_adc_values.pop();
    return m_current_value;
  }

  std::queue<percentage> m_adc_values{};
};
/** @} */
}  // namespace hal::mock
