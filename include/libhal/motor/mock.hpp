#pragma once

#include <concepts>

#include "../testing.hpp"
#include "interface.hpp"

namespace hal::mock {
/**
 * @addtogroup motor
 * @{
 */
/**
 * @brief Mock motor implementation for use in unit tests and simulations with a
 * spy function for power()
 *
 */
template<std::floating_point float_t = config::float_type>
struct motor : public hal::motor<float_t>
{
  /**
   * @brief Reset spy information for power()
   *
   */
  void reset()
  {
    spy_power.reset();
  }

  /// Spy handler for hal::motor::write()
  spy_handler<percentage<float_t>> spy_power;

private:
  status driver_power(percentage<float_t> p_power) noexcept override
  {
    return spy_power.record(p_power);
  };
};
/** @} */
}  // namespace hal::mock
