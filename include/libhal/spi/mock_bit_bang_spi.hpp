
#pragma once
#include "../testing.hpp"
#include "bit_bang_spi.hpp"

namespace hal::mock {
/**
 * @addtogroup bit_bang_spi Bit Bang SPI
 *
 */
/**
 * @brief mock bit bang spi for use in unit tests and simulations
 *
 */

struct bit_bang_spi : public hal::bit_bang_spi
{
  /**
   * @brief
   *
   */
  /*
  void reset()
  {
    spy_configure.reset();
  }

  /// Spy handler for hal::bit_bang_spi::configure()
  spy_handler<settings> spy_configure;

private:
  boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  }
  */
};
}  // namespace hal::mock
