#pragma once

#include "../adc/interface.hpp"
#include "../math.hpp"

namespace hal {
/**
 * @brief Linear potentiometer driver that takes an hal::adc and a
 * settings struct, and reads the distance the linear potentiometer has traveled
 * in micrometer.
 *
 */
class linear_potentiometer
{
public:
  /// Universal unit for length
  using micrometer = std::int32_t;
  /**
   * @brief Settings for a linear potentiometer object. Contains
   * variables for minimum and maximum travel distance, as well as variables for
   * minimum and maximum percentages for the ADC.
   *
   */
  struct settings
  {
    /// Minimum travel distance in micrometer
    micrometer p_min_distance = 0;
    /// Maximum travel distance in micrometer
    micrometer p_max_distance = 0;
    /// Minimum percent that can be read from ADC
    hal::percent p_min_adc_output = hal::percent::from_ratio(0, 1);
    /// Maximum percent that can be read from ADC
    hal::percent p_max_adc_output = hal::percent::from_ratio(1, 1);
  };

  /**
   * @brief Construct a new linear potentiometer object.
   *
   * @param p_adc ADC of linear potentiometer.
   * @param p_settings Settings for linear potentiometer.
   */
  linear_potentiometer(hal::adc& p_adc, settings p_settings)
    : m_adc(&p_adc)
    , m_settings(p_settings)
  {
  }

  /**
   * @brief Read the distance the linear potentiometer has traveled.
   *
   * @return boost::leaf::result<length> - the distance traveled of the
   * potentiometer in micrometer.
   */
  boost::leaf::result<micrometer> read()
  {
    auto adc_percent = BOOST_LEAF_CHECK(m_adc->read());
    auto scaled_percent = adc_percent.scale(m_settings.p_min_adc_output,
                                            m_settings.p_max_adc_output);
    auto scaled_distance = scaled_percent.scale(m_settings.p_min_distance,
                                                m_settings.p_max_distance);
    return scaled_distance;
  }

private:
  hal::adc* m_adc = nullptr;
  settings m_settings{};
};
}  // namespace hal
