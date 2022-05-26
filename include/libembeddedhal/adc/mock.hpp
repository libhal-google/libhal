#include "interface.hpp"

namespace embed::mock {
/**
 * @brief Mock adc implementation for use in unit tests and simulations.
 *
 */
struct adc : public embed::adc
{
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
}  // namespace embed::mock
