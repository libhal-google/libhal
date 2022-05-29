#pragma once

#include <units/isq/si/length.h>

#include "../adc/interface.hpp"
#include "../math.hpp"

namespace embed {
/**
 * @brief Linear potentiometer driver that takes an embed::adc and a
 * settings struct, and reads the distance the linear potentiometer has traveled
 * in nanometres.
 *
 */
class linear_potentiometer
{
public:
  /// Universal unit for length
  using length =
    units::isq::si::length<units::isq::si::nanometre, std::int64_t>;
  /**
   * @brief Settings for a linear potentiometer object. Contains
   * variables for minimum and maximum travel distance, as well as variables for
   * minimum and maximum percentages for the ADC.
   *
   */
  struct settings
  {
    /// Minimum travel distance in nanometres
    length p_min_distance = units::aliases::isq::si::nm<std::int64_t>(0);
    /// Maximum travel distance in nanometres
    length p_max_distance = units::aliases::isq::si::nm<std::int64_t>(0);
    /// Minimum percent that can be read from ADC
    embed::percent p_min_adc_output = embed::percent::from_ratio(0, 1);
    /// Maximum percent that can be read from ADC
    embed::percent p_max_adc_output = embed::percent::from_ratio(1, 1);
  };

  /**
   * @brief Construct a new linear potentiometer object.
   *
   * @param p_adc ADC of linear potentiometer.
   * @param p_settings Settings for linear potentiometer.
   */
  linear_potentiometer(embed::adc& p_adc, settings p_settings)
    : m_adc(&p_adc)
    , m_settings(p_settings)
  {}

  /**
   * @brief Read the distance the linear potentiometer has traveled.
   *
   * @return boost::leaf::result<length> - the distance traveled of the
   * potentiometer in nanometres.
   */
  boost::leaf::result<length> read()
  {
    return units::aliases::isq::si::nm<std::int64_t>(linear_map(
      static_cast<std::int64_t>(m_adc->read().value().raw_value()),
      static_cast<std::int64_t>(m_settings.p_min_adc_output.raw_value()),
      static_cast<std::int64_t>(m_settings.p_max_adc_output.raw_value()),
      m_settings.p_min_distance.number(),
      m_settings.p_max_distance.number()));
  }

private:
  embed::adc* m_adc = nullptr;
  settings m_settings{};
};
}  // namespace embed
