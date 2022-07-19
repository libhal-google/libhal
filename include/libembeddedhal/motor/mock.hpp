#pragma once

#include "../testing.hpp"
#include "interface.hpp"

namespace embed::mock {
/**
 * @addtogroup motor
 * @{
 */
/**
 * @brief Mock motor implementation for use in unit tests and simulations with a
 * spy function for power()
 *
 */
struct motor : public embed::motor_interface
{
  /**
   * @brief Reset spy information for power()
   *
   */
  void reset()
  {
    spy_power.reset();
  }

  /// Spy handler for embed::motor_interface::write()
  spy_handler<percent> spy_power;

private:
  boost::leaf::result<void> driver_power(percent p_power) noexcept override
  {
    return spy_power.record(p_power);
  };
};
/** @} */
}  // namespace embed::mock
