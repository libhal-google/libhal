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
template<std::floating_point float_t = config::float_type>
struct servo : public hal::servo<float_t>
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
  spy_handler<percentage<float_t>> spy_position;

private:
  status driver_position(percentage<float_t> p_position) noexcept override
  {
    return spy_position.record(p_position);
  }
};
/** @} */
}  // namespace hal::mock
