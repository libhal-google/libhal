#pragma once

#include <concepts>

#include "../testing.hpp"
#include "interface.hpp"

namespace hal::mock {
/**
 * @addtogroup pwm
 * @{
 */
/**
 * @brief Mock pwm implementation for use in unit tests and simulations with spy
 * functions for configure() and duty_cycle().
 *
 */
template<std::floating_point float_t = config::float_type>
struct pwm : public hal::pwm<float_t>
{
  using settings = hal::pwm<float_t>::settings;
  /**
   * @brief Reset spy information for both configure() and duty_cycle()
   *
   */
  void reset()
  {
    spy_configure.reset();
    spy_duty_cycle.reset();
  }

  /// Spy handler for hal::pwm::configure()
  spy_handler<settings> spy_configure;
  /// Spy handler for hal::pwm::duty_cycle()
  spy_handler<percentage<float_t>> spy_duty_cycle;

private:
  status driver_configure(const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  };
  status driver_duty_cycle(percentage<float_t> p_duty_cycle) noexcept override
  {
    return spy_duty_cycle.record(p_duty_cycle);
  };
};
/** @} */
}  // namespace hal::mock
