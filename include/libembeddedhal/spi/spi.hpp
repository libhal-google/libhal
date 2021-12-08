#pragma once

#include "../driver.hpp"
#include "inactive.hpp"

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
  /**
   * @brief Serial clock frequency
   *
   */
  uint32_t clock_rate_hz = 100'000;
  /**
   * @brief The polarity of the pins when the signal is idle
   *
   */
  bool clock_idles_high = false;
  /**
   * @brief The phase of the clock signal when communicating
   *
   */
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
   * @brief Determines if a transfer is currently ongoing.
   *
   * @return true transfer is currently ongoing
   * @return false device is idle and available for a transaction
   */
  [[nodiscard]] virtual bool busy() = 0;
  /**
   * @brief Send and receieve bytes from the spi bus.
   *
   * @param p_data_out buffer to write data to the bus. If this is set to
   * null/empty then writing is ignored and the p_write_value will be written to
   * the bus. If the length is less than p_data_in, then p_write_value will be
   * written to the bus after this buffer has been sent.
   * @param p_data_in buffer to read the data off of the bus. If this is
   * null/empty, then the transfer will be write only and the incoming data will
   * be ignored. If the length of this buffer is less than p_data_out, once this
   * buffer has been filled, the rest of the received bytes on the bus will be
   * dropped.
   * @param p_write_value filler data to be placed on the bus when the
   * p_data_out buffer has been exhausted.
   */
  virtual void transfer(std::span<const std::byte> p_data_out,
                        std::span<std::byte> p_data_in,
                        std::byte p_write_value = std::byte{ 0xFF }) = 0;
};
}  // namespace embed
