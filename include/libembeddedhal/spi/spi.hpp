#pragma once

#include "../driver.hpp"
#include "inactive.hpp"

#include <cinttypes>
#include <cstddef>
#include <span>

namespace embed {

/// Generic settings for a standard SPI peripheral
struct spi_settings
{
  /// Serial clock frequency
  uint32_t clock_rate_hz = 100'000;
  /// The polarity of the pins when the signal is idle
  bool clock_idles_high = false;
  /// The phase of the clock signal when communicating
  bool data_valid_on_trailing_edge = false;
};

/// An abstract interface for hardware that implements the Serial Peripheral
/// Interface (SPI) communication protocol.
class spi : public driver<spi_settings>
{
public:
  [[nodiscard]] virtual bool busy() = 0;
  virtual void transfer(std::span<std::byte> p_data_out,
                        std::span<std::byte> p_data_in) = 0;
};
} // namespace embed
