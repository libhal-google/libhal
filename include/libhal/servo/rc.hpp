// Use "#pragma once" as an include guard for headers
// This is required because it ensures that the compiler will process this file
// only once, no matter how many times it is included.
#pragma once

// Include headers in the following order:
// Related header, C system headers, C++ standard library headers,
// other libraries' headers, libhal headers
#include "../map.hpp"
#include "../pwm/interface.hpp"
#include "interface.hpp"

// Keep driver within the namespace hal to not pollute global namespace
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
  // Use a factory function in place of a constructor to report any errors that
  // could happen and let the application decide the next steps. The Factory
  // function returns a result<rc_servo> object.
  // The standard name for factory functions for device drivers is "create()"
  // and the signature must be:
  // static result<type> create(/* ... args ... */){ /* ... */ }
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

    // Check if any errors happened while setting the frequency of the pwm.
    // Using HAL_CHECK will return any errors that occur from the factory
    // function allowing the caller to choose what to do with the error
    // information.
    HAL_CHECK(p_pwm.frequency(frequency));

    // Calculate the wavelength in microseconds.
    auto wavelength = (1.0f / frequency) * std::micro::den;
    // min_percent represents the minimum percentage to be used with the pwm
    // signal. The percentage is calculated by using the minimum width of the
    // signal in microseconds divided by the wavelength to get the decimal
    // representation of the percentage.
    auto min_percent = percentage(MinMicroseconds / wavelength);
    // max_percent represents the maximum percentage to be used with the pwm
    // signal. The percentage is calculated by using the maximum width of the
    // signal in microseconds divided by the wavelength to get the decimal
    // representation of the percentage.
    auto max_percent = percentage(MaxMicroseconds / wavelength);
    // percent_range holds float value of min_percent and max_percent for use
    // with map function used in position().
    auto percent_range =
      std::make_pair(min_percent.value(), max_percent.value());
    // If no errors happen, call the constructor with verified parameters
    return rc_servo(p_pwm, percent_range);
  }

private:
  // Constructor is private to only be accessed from the factory function.
  // Use p_ prefix for function parameters.
  // Use an initializer list to initialize private members.
  constexpr rc_servo(hal::pwm& p_pwm, std::pair<float, float> p_percent_range)
    : m_pwm(&p_pwm)
    , m_percent_range(p_percent_range)
  {
  }

  // Drivers must implement functions that are listed in interface. Use override
  // keyword for virtual functions
  status driver_position(percentage p_position) noexcept override
  {
    // The range of p_position is from -100% to 100%. The value of p_position is
    // mapped within the percentage range of the pwm signal to get the decimal
    // value of the scaled_percentage.
    //
    // Example:
    // pwm min percentage: percentage(0.05)
    // pwm max percentage: percentage(0.25)
    //
    // percentage(0.05) = position(percentage(-1.0))
    // percentage(0.10) = position(percentage(-0.5))
    // percentage(0.20) = position(percentage(0.5))
    // percentage(0.25) = position(percentage(1.0))
    auto scaled_percent_raw =
      map(p_position.value(),
          std::make_pair(hal::percentage::min, hal::percentage::max),
          m_percent_range);
    auto scaled_percent = percentage(scaled_percent_raw);
    // Set the duty cycle of the pwm with the scaled percent.
    return m_pwm->duty_cycle(scaled_percent);
  }

  // Use m_ prefix for private/protected class members.
  // Use a pointer here rather than a reference, because member references
  // implicitly delete move constructors
  hal::pwm* m_pwm;
  std::pair<float, float> m_percent_range;
};
/** @} */
// Comment the end of the namepace and end the file with an extra line.
}  // namespace hal
