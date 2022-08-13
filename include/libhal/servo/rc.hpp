#pragma once

#include "../frequency.hpp"
#include "../map.hpp"
#include "../pwm/interface.hpp"
#include "interface.hpp"

namespace hal {
/**
 * @addtogroup servo
 * @{
 */
/**
 * @brief Generic RC servo driver.
 *
 */
class rc_servo : public hal::servo
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
   * @return result<rc_servo> - Constructed rc_servo object
   */
  template<uint32_t Frequency = 50,
           uint32_t MinMicroseconds = 1000,
           uint32_t MaxMicroseconds = 2000>
  static result<rc_servo> create(hal::pwm& p_pwm)
  {
    // Check that MinMicroseconds is less than MaxMicroseconds
    static_assert(
      MinMicroseconds < MaxMicroseconds,
      "The minimum microseconds must be less than maximum microseconds.");
    // Check that Frequency period is greater than MaxMicroseconds
    static_assert(
      (1000.0 / Frequency) > (MaxMicroseconds / 1000.0),
      "The maximum microseconds is greater than the period of the frequency");
    constexpr auto frequency = hertz{ Frequency };

    HAL_CHECK(p_pwm.frequency(frequency));

    auto wavelength = (1.0f / frequency) * std::micro::den;
    auto min_percent = percentage(MinMicroseconds / wavelength);
    auto max_percent = percentage(MaxMicroseconds / wavelength);
    auto percent_range =
      std::make_pair(min_percent.value(), max_percent.value());

    return rc_servo(p_pwm, percent_range);
  }

private:
  constexpr rc_servo(hal::pwm& p_pwm, std::pair<float, float> p_percent_range)
    : m_pwm(&p_pwm)
    , m_percent_range(p_percent_range)
  {
  }

  status driver_position(percentage p_position) noexcept override
  {
    auto scaled_percent_raw =
      map(p_position.value(),
          std::make_pair(hal::percentage::min, hal::percentage::max),
          m_percent_range);
    auto scaled_percent = percentage(scaled_percent_raw);
    return m_pwm->duty_cycle(scaled_percent);
  }

  hal::pwm* m_pwm;
  std::pair<float, float> m_percent_range;
};
/** @} */
}  // namespace hal
