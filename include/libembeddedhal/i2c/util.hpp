/**
 * @file util.hpp
 */
#pragma once

#include <functional>

#include "../error.hpp"
#include "interface.hpp"

namespace hal {
/**
 * @addtogroup i2c
 * @{
 */
/**
 * @brief write data to a target device on the i2c bus
 *
 * Shorthand for writing i2c.transfer(...) for write only operations
 *
 * @param p_i2c - i2c driver
 * @param p_address - target address
 * @param p_data_out - buffer of bytes to write to the target device
 * @param p_timeout - amount of time to execute the transaction
 * @return boost::leaf::result<void> - any errors associated with the read call
 */
[[nodiscard]] inline boost::leaf::result<void> write(
  i2c& p_i2c,
  std::byte p_address,
  std::span<const std::byte> p_data_out,
  std::function<hal::timeout> p_timeout = hal::never_timeout()) noexcept
{
  return p_i2c.transaction(
    p_address, p_data_out, std::span<std::byte>{}, p_timeout);
}
/**
 * @brief read bytes from target device on i2c bus
 *
 * Shorthand for writing i2c.transfer(...) for read only operations
 *
 * @param p_i2c - i2c driver
 * @param p_address - target address
 * @param p_data_in - buffer to read bytes into from target device
 * @param p_timeout - amount of time to execute the transaction
 * @return boost::leaf::result<void> - any errors associated with the read call
 */
[[nodiscard]] inline boost::leaf::result<void> read(
  i2c& p_i2c,
  std::byte p_address,
  std::span<std::byte> p_data_in,
  std::function<hal::timeout> p_timeout = hal::never_timeout()) noexcept
{
  return p_i2c.transaction(
    p_address, std::span<std::byte>{}, p_data_in, p_timeout);
}
/**
 * @brief return array of read bytes from target device on i2c bus
 *
 * Eliminates the need to create a buffer and pass it into the read function.
 *
 * @tparam BytesToRead - number of bytes to read
 * @param p_i2c - i2c driver
 * @param p_address - target address
 * @param p_timeout - amount of time to execute the transaction
 * @return boost::leaf::result<std::array<std::byte, BytesToRead>> - array of
 * bytes from target device or an error.
 */
template<size_t BytesToRead>
[[nodiscard]] boost::leaf::result<std::array<std::byte, BytesToRead>> read(
  i2c& p_i2c,
  std::byte p_address,
  std::function<hal::timeout> p_timeout = hal::never_timeout()) noexcept
{
  std::array<std::byte, BytesToRead> buffer;
  BOOST_LEAF_CHECK(read(p_i2c, p_address, buffer, p_timeout));
  return buffer;
}
/**
 * @brief write and then read bytes from target device on i2c bus
 *
 * This API simply calls transaction. This API is here for consistency across
 * the other other communication protocols such as SPI and serial.
 *
 * @param p_i2c - i2c driver
 * @param p_address - target address
 * @param p_data_out - buffer of bytes to write to the target device
 * @param p_data_in - buffer to read bytes into from target device
 * @param p_timeout - amount of time to execute the transaction
 *
 * @return boost::leaf::result<void> - any errors associated with the read call
 */
[[nodiscard]] inline boost::leaf::result<void> write_then_read(
  i2c& p_i2c,
  std::byte p_address,
  std::span<const std::byte> p_data_out,
  std::span<std::byte> p_data_in,
  std::function<hal::timeout> p_timeout = hal::never_timeout()) noexcept
{
  return p_i2c.transaction(p_address, p_data_out, p_data_in, p_timeout);
}
/**
 * @brief write and then return an array of read bytes from target device on i2c
 * bus
 *
 * Eliminates the need to create a buffer and pass it into the read function.
 *
 * @tparam BytesToRead - number of bytes to read after write
 * @param p_i2c - i2c driver
 * @param p_address - target address
 * @param p_data_out - buffer of bytes to write to the target device
 * @param p_timeout - amount of time to execute the transaction
 * @return boost::leaf::result<std::array<std::byte, BytesToRead>>
 */
template<size_t BytesToRead>
[[nodiscard]] boost::leaf::result<std::array<std::byte, BytesToRead>>
write_then_read(
  i2c& p_i2c,
  std::byte p_address,
  std::span<const std::byte> p_data_out,
  std::function<hal::timeout> p_timeout = hal::never_timeout()) noexcept
{
  std::array<std::byte, BytesToRead> buffer;
  BOOST_LEAF_CHECK(
    write_then_read(p_i2c, p_address, p_data_out, buffer, p_timeout));
  return buffer;
}
/// @}
}  // namespace hal
