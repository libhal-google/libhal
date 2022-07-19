#pragma once

#include "../testing.hpp"
#include "interface.hpp"

namespace embed::mock {
/**
 * @addtogroup dac
 * @{
 */
/**
 * @brief Mock dac implementation for use in unit tests and simulations with a
 * spy function for write()
 *
 */
struct dac : public embed::dac_interface
{
  /**
   * @brief Reset spy information for write()
   *
   */
  void reset()
  {
    spy_write.reset();
  }

  /// Spy handler for embed::dac_interface::write()
  spy_handler<percent> spy_write;

private:
  boost::leaf::result<void> driver_write(percent p_value) noexcept override
  {
    return spy_write.record(p_value);
  };
};
/** @} */
}  // namespace embed::mock
