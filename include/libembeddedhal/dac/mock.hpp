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
struct dac : public hal::dac
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
  spy_handler<percent> spy_write;

private:
  boost::leaf::result<void> driver_write(percent p_value) noexcept override
  {
    return spy_write.record(p_value);
  };
};
/** @} */
}  // namespace hal::mock
