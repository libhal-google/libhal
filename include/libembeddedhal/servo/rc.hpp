#pragma once

#include "../frequency.hpp"
#include "../map.hpp"
#include "../pwm/interface.hpp"
#include "interface.hpp"

namespace embed {
/**
 * @addtogroup servo
 * @{
 */
/**
 * @brief Generic RC servo driver.
 *
 */
class rc_servo : public embed::servo
{
public:
  /**
   * @brief Factory function to create a rc_servo object.
   *
   * Compile time error will occur if the min microseconds is more than the max
   * microseconds, or if the max microseconds is more than the frequency
   * wavelength.
   *
   * @tparam Frequency - Frequency to configure pwm
   * @tparam MinMicroseconds - Minimum pulse width
   * @tparam MaxMicroseconds - Maximum pulse width
   * @param p_pwm - Pwm signal of rc_servo
   * @return boost::leaf::result<rc_servo> - Constructed rc_servo object
   */
  template<uint32_t Frequency = 50,
           uint32_t MinMicroseconds = 1000,
           uint32_t MaxMicroseconds = 2000>
  static boost::leaf::result<rc_servo> create(embed::pwm& p_pwm)
  {
    // Check that MinMicroseconds is less than MaxMicroseconds
    static_assert(
      MinMicroseconds < MaxMicroseconds,
      "The minimum microseconds must be less than maximum microseconds.");
    // Check that Frequency period is greater than MaxMicroseconds
    static_assert(
      (1000.0 / Frequency) > (MaxMicroseconds / 1000.0),
      "The maximum microseconds is greater than the period of the frequency");
    auto frequency = embed::frequency{ Frequency };
    embed::pwm::settings settings{ .frequency = frequency };
    BOOST_LEAF_CHECK(p_pwm.configure(settings));

    auto frequency_wavelength =
      static_cast<uint32_t>(wavelength<std::micro>(frequency).count());
    auto min_percent =
      percent::from_ratio(MinMicroseconds, frequency_wavelength);
    auto max_percent =
      percent::from_ratio(MaxMicroseconds, frequency_wavelength);
    return rc_servo(p_pwm, frequency, min_percent, max_percent);
  }

private:
  constexpr rc_servo(embed::pwm& p_pwm,
                     frequency p_frequency,
                     percent p_min_percent,
                     percent p_max_percent)
    : m_pwm(&p_pwm)
    , m_frequency(p_frequency)
    , m_min_percent(p_min_percent)
    , m_max_percent(p_max_percent)
  {
  }

  boost::leaf::result<void> driver_position(
    percent p_position) noexcept override
  {
    auto scaled_percent_raw = BOOST_LEAF_CHECK(
      map(p_position.raw_value() / 2,
          { .x = percent::raw_min() / 2, .y = percent::raw_max() / 2 },
          { .x = m_min_percent.raw_value(), .y = m_max_percent.raw_value() }));
    auto scaled_percent =
      percent::from_ratio(static_cast<int32_t>(scaled_percent_raw),
                          static_cast<int32_t>(percent::raw_max()));
    return m_pwm->duty_cycle(scaled_percent);
  }

  embed::pwm* m_pwm;
  embed::frequency m_frequency = frequency(50);
  percent m_min_percent = percent::from_ratio(1, 10);
  percent m_max_percent = percent::from_ratio(2, 10);
};
/** @} */
}  // namespace embed
