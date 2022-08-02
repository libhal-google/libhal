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
template<std::floating_point float_t = config::float_type>
struct adc : public hal::adc<float_t>
{
  /**
   * @brief Queues the percentages to be returned for read()
   *
   * @param p_adc_values - queue of percentages
   */
  void set(std::queue<percentage<float_t>>& p_adc_values)
  {
    m_adc_values = p_adc_values;
  }

private:
  result<percentage<float_t>> driver_read() noexcept
  {
    if (m_adc_values.size() == 0) {
      return boost::leaf::new_error(
        std::out_of_range("percentages queue is empty!"));
    }
    percentage<float_t> m_current_value = m_adc_values.front();
    m_adc_values.pop();
    return m_current_value;
  }

  std::queue<percentage<float_t>> m_adc_values{};
};
/** @} */
}  // namespace hal::mock
