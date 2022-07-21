#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

#include "../error.hpp"
#include "../frequency.hpp"

namespace embed {
/**
 * @addtogroup spi
 * Available spi APIs
 * @{
 */
/**
 * @brief Serial peripheral interface (SPI) communication protocol hardware
 * abstract interface
 *
 */
class spi
{
public:
  /// Generic settings for a standard SPI device.
  struct settings
  {
    /// Serial clock frequency
    frequency clock_rate{};
    /// The polarity of the pins when the signal is idle
    bool clock_idles_high = false;
    /// The phase of the clock signal when communicating
    bool data_valid_on_trailing_edge = false;

    /**
     * @brief Default operators for <, <=, >, >= and ==
     *
     * @return auto - result of the comparison
     */
    [[nodiscard]] constexpr auto operator<=>(const settings&) const noexcept =
      default;
  };

  /// Default filler data placed on the bus in place of actual write data when
  /// the write buffer has been exhausted.
  static constexpr std::byte default_filler = std::byte{ 0xFF };

  /**
   * @brief Configure spi to match the settings supplied
   *
   * @param p_settings - settings to apply to spi
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation. Will return embed::error::invalid_settings if the settings could
   * not be achieved.
   */
  [[nodiscard]] boost::leaf::result<void> configure(
    const settings& p_settings) noexcept
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
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  [[nodiscard]] boost::leaf::result<void> transfer(
    std::span<const std::byte> p_data_out,
    std::span<std::byte> p_data_in,
    std::byte p_filler = default_filler) noexcept
  {
    return driver_transfer(p_data_out, p_data_in, p_filler);
  }

private:
  virtual boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept = 0;
  virtual boost::leaf::result<void> driver_transfer(
    std::span<const std::byte> p_data_out,
    std::span<std::byte> p_data_in,
    std::byte p_filler) noexcept = 0;
};
/** @} */
}  // namespace embed
