#include "../testing.hpp"
#include "motor.hpp"

namespace embed::mock {
/**
 * @brief Mock motor implementation for use in unit tests and simulations with a
 * spy function for power()
 *
 */
struct motor : public embed::motor
{
  /**
   * @brief Reset spy information for power()
   *
   */
  void reset()
  {
    spy_power.reset();
  }

  /// Spy handler for embded::motor::write()
  spy_handler<percent> spy_power;

private:
  boost::leaf::result<void> driver_power(percent p_power) noexcept override
  {
    return spy_power.record(p_power);
  };
};
}  // namespace embed::mock
