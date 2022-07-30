#pragma once

#include "../testing.hpp"
#include "interface.hpp"

namespace hal::mock {
/**
 * @addtogroup dac
 * @{
 */
/**
 * @brief Mock dac implementation for use in unit tests and simulations with a
 * spy function for write()
 *
 */

template<std::floating_point float_t = config::float_type>
struct dac : public hal::dac<float_t>
{
  /**
   * @brief Reset spy information for write()
   *
   */
  void reset()
  {
    spy_write.reset();
  }

  /// Spy handler for hal::dac::write()
  spy_handler<percentage<float_t>> spy_write;

private:
  status driver_write(percentage<float_t> p_value) noexcept override
  {
    return spy_write.record(p_value);
  };
};
/** @} */
}  // namespace hal::mock
