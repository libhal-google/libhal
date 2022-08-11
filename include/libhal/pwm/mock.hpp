#pragma once

#include "../testing.hpp"
#include "interface.hpp"

namespace hal::mock {
/**
 * @addtogroup pwm
 * @{
 */
/**
 * @brief Mock pwm implementation for use in unit tests and simulations with spy
 * functions for frequency() and duty_cycle().
 *
 */
template<std::floating_point float_t = config::float_type>
struct pwm : public hal::pwm<float_t>
{
  /**
   * @brief Reset spy information for both frequency() and duty_cycle()
   *
   */
  void reset()
  {
    spy_frequency.reset();
    spy_duty_cycle.reset();
  }

  /// Spy handler for hal::pwm::frequency()
  spy_handler<hertz> spy_frequency;
  /// Spy handler for hal::pwm::duty_cycle()
  spy_handler<percentage<float_t>> spy_duty_cycle;

private:
  status driver_frequency(hertz p_settings) noexcept override
  {
    return spy_frequency.record(p_settings);
  }
  status driver_duty_cycle(percentage<float_t> p_duty_cycle) noexcept override
  {
    return spy_duty_cycle.record(p_duty_cycle);
  }
};
/** @} */
}  // namespace hal::mock
