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
   * @brief
   *
   * @param p_data_out
   * @param p_data_in
   */
  virtual void transfer(std::span<const std::byte> p_data_out,
                        std::span<std::byte> p_data_in) = 0;
};
} // namespace embed
