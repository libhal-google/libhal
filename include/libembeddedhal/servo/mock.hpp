#pragma once

#include "../testing.hpp"
#include "interface.hpp"

namespace embed::mock {
/**
 * @addtogroup servo
 * @{
 */
/**
 * @brief mock servo implementation for use in unit tests and
 * simulations.
 *
 */
struct servo : public embed::servo
{
  /**
   * @brief Reset spy information for position()
   *
   */
  void reset()
  {
    spy_position.reset();
  }

  /// Spy handler for embed::servo::position()
  spy_handler<embed::percent> spy_position;

private:
  boost::leaf::result<void> driver_position(
    percent p_position) noexcept override
  {
    return spy_position.record(p_position);
  }
};
/** @} */
}  // namespace embed::mock
