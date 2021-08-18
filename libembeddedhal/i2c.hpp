#pragma once

#include "driver.hpp"
#include "full_scale.hpp"

#include <cinttypes>
#include <cstddef>
#include <span>

namespace embed {
/// Generic settings for a standard I2C peripheral
struct i2c_settings
{
  /// Operating frequency
  uint32_t clock_rate_hz = 100'000;
  /// Clock duty cycle, default to 50% which is represented as 0.
  /// Positive values increase the duty cycle above 50% up to +127
  /// Negative values decrease the duty cycle below 50% down to -128
  full_scale<int8_t> duty_cycle = bit_depth<int8_t, 8>(0);
};

/// An abstract interface for hardware that implements the Inter-integrated
/// Circuit (I2C) or Two Wire Interface (TWI) hardware communication Protocol.
class i2c : public driver<i2c_settings>
{
public:
  [[nodiscard]] virtual bool busy() = 0;

  // https://www.i2c-bus.org/addressing/10-bit-addressing
  virtual void transaction(uint8_t p_address,
                           std::span<const std::byte> p_data_out,
                           std::span<std::byte> p_data_in) = 0;
};
} // namespace embed
