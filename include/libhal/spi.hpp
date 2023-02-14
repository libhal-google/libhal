#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

#include "config.hpp"
#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief Serial peripheral interface (SPI) communication protocol hardware
 * abstract interface
 *
 */
class spi
{
public:
  /**
   * @brief Generic settings for a standard SPI device.
   *
   */
  struct settings
  {
    /**
     * @brief Serial clock frequency in hertz
     *
     */
    hertz clock_rate = 100.0_kHz;
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
   * @brief Feedback from performing a transfer on the spi bus
   *
   * This structure is currently empty as no feedback has been determined for
   * now. This structure may be expanded in the future.
   */
  struct transfer_t
  {};

  /// Default filler data placed on the bus in place of actual write data when
  /// the write buffer has been exhausted.
  static constexpr hal::byte default_filler = hal::byte{ 0xFF };

  /**
   * @brief Configure spi to match the settings supplied
   *
   * @param p_settings - settings to apply to spi
   * @return status - success or failure
   * @throws std::errc::invalid_argument if the settings could not be achieved.
   */
  [[nodiscard]] status configure(const settings& p_settings)
  {
    return driver_configure(p_settings);
  }
  /**
   * @brief Send and receive data between a selected device on the spi bus.
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
   * @return result<transfer_t> - success or failure
   */
  [[nodiscard]] result<transfer_t> transfer(
    std::span<const hal::byte> p_data_out,
    std::span<hal::byte> p_data_in,
    hal::byte p_filler = default_filler)
  {
    return driver_transfer(p_data_out, p_data_in, p_filler);
  }

  virtual ~spi() = default;

private:
  virtual status driver_configure(const settings& p_settings) = 0;
  virtual result<transfer_t> driver_transfer(
    std::span<const hal::byte> p_data_out,
    std::span<hal::byte> p_data_in,
    hal::byte p_filler) = 0;
};
}  // namespace hal
