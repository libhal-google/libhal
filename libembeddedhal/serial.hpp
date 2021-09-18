#pragma once

#include "driver.hpp"

#include <chrono>
#include <cinttypes>
#include <cstddef>
#include <span>

namespace embed {
/// Generic settings for a standard UART peripheral
struct serial_settings
{
  /// Defines the available stop bits options
  enum class stop_bits : uint8_t
  {
    one = 0,
    two,
  };

  /// Defines the parity bit options
  enum class parity : uint8_t
  {
    /// Disable parity checks
    none = 0,
    /// Enable parity and set HIGH when the number of bits is odd
    odd,
    /// Enable parity and set HIGH when the number of bits is even
    even,
    forced1,
    forced0,
  };

  uint32_t baud_rate = 115200;
  parity parity = parity::none;
  stop_bits stop = stop_bits::one;
  uint8_t frame_size = 8;
};

/// An abstract interface for hardware that implements a serial protocol like
/// UART, RS485, RS232 etc.
class serial : public driver<serial_settings>
{
public:
  [[nodiscard]] virtual bool busy() = 0;
  virtual void write(std::span<const std::byte> p_data) = 0;

  [[nodiscard]] virtual size_t bytes_available() = 0;
  virtual void read(std::span<std::byte> p_data) = 0;
  virtual void flush() = 0;
};
} // namespace embed
