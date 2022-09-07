#pragma once

#include "../testing.hpp"
#include "interface.hpp"

namespace hal::mock {
/**
 * @addtogroup servo
 * @{
 */
/**
 * @brief mock servo implementation for use in unit tests and
 * simulations.
 *
 */
struct servo : public hal::servo
{
  /**
   * @brief Reset spy information for position()
   *
   */
  void reset()
  {
    spy_position.reset();
  }

  /// Spy handler for hal::servo::position()
  spy_handler<float> spy_position;

private:
  status driver_position(float p_position) noexcept override
  {
    return spy_position.record(p_position);
  }
};
/** @} */
}  // namespace hal::mock
