#pragma once

#include "../driver.hpp"
#include "../frequency.hpp"

#include <cinttypes>
#include <cstddef>
#include <span>

namespace embed {
/**
 * @brief Generic settings for a standard SPI device.
 *
 */
struct spi_settings
{
  /// Serial clock frequency
  frequency clock_rate = default_clock_rate;
  /// The polarity of the pins when the signal is idle
  bool clock_idles_high = false;
  /// The phase of the clock signal when communicating
  bool data_valid_on_trailing_edge = false;
};

/**
 * @brief Serial peripheral interface (SPI) communication protocol hardware
 * abstract interface
 *
 */
class spi : public driver<spi_settings>
{
public:
  /**
   * @brief Send and receieve data between a selected device on the spi bus.
   * This function will block until the entire transfer is finished.
   *
   * @param p_data_out - buffer to write data to the bus. If this is set to
   * null/empty then writing is ignored and the p_filler will be written to
   * the bus. If the length is less than p_data_in, then p_filler will be
   * written to the bus after this buffer has been sent.
   * @param p_data_in - buffer to read the data off of the bus. If this is
   * null/empty, then the transfer will be write only and the incoming data will
   * be ignored. If the length of this buffer is less than p_data_out, once this
   * buffer has been filled, the rest of the received bytes on the bus will be
   * dropped.
   * @param p_filler - filler data placed on the bus in place of actual write
   * data when p_data_out has been exhausted.
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  virtual boost::leaf::result<void> transfer(
    std::span<const std::byte> p_data_out,
    std::span<std::byte> p_data_in,
    std::byte p_filler) = 0;
};
}  // namespace embed
