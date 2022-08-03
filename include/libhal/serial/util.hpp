#pragma once

#include <optional>
#include <span>
#include <string_view>

#include "../error.hpp"
#include "../units.hpp"
#include "interface.hpp"

namespace hal {
/**
 * @addtogroup serial
 * @{
 */
/**
 * @brief Delay execution until the serial buffer has reached a specific number
 * of buffered bytes.
 *
 * NOTE: If the length is greater than the serial port's capacity this will loop
 * forever.
 *
 * @param p_serial - serial port to wait for
 * @param p_length - the number of bytes that need to be buffered before this
 * function returns.
 * @return status - return an error if a call to
 * serial::bytes_available returns an error from the serial port.
 */
[[nodiscard]] inline status delay(serial& p_serial, size_t p_length) noexcept
{
  size_t bytes_available = HAL_CHECK(p_serial.bytes_available());
  while (bytes_available < p_length) {
    bytes_available = HAL_CHECK(p_serial.bytes_available());
  }
  return {};
}

/**
 * @brief Write bytes to a serial port
 *
 * @param p_serial - the serial port that will be written to
 * @param p_data_out - the data to be written out the port
 * @return status - return an error if a call to
 * serial::write returns an error from the serial port.
 */
[[nodiscard]] inline status write(
  serial& p_serial,
  std::span<const hal::byte> p_data_out) noexcept
{
  return p_serial.write(p_data_out);
}

/**
 * @brief Read bytes from a serial port
 *
 * @param p_serial - the serial port that will be read from
 * @param p_data_in - buffer to have bytes from the serial port read into
 * @return result<std::span<const hal::byte>> - return an error if
 * a call to serial::read or delay() returns an error from the serial port or
 * a span with the number of bytes read and a pointer to where the read bytes
 * are.
 */
[[nodiscard]] inline result<std::span<const hal::byte>> read(
  serial& p_serial,
  std::span<hal::byte> p_data_in)
{
  HAL_CHECK(delay(p_serial, p_data_in.size()));
  return p_serial.read(p_data_in);
}

/**
 * @brief Read bytes from a serial port and return an array.
 *
 * This call eliminates the boiler plate of creating an array and then passing
 * that to the read function.
 *
 * @tparam BytesToRead - the number of bytes to be read from the serial port.
 * @param p_serial - the serial port to be read from
 * @return result<std::array<hal::byte, BytesToRead>> - return an
 * error if a call to serial::read or delay() returns an error from the
 * serial port or a span with the number of bytes read and a pointer to where
 * the read bytes are.
 */
template<size_t BytesToRead>
[[nodiscard]] result<std::array<hal::byte, BytesToRead>> read(
  serial& p_serial) noexcept
{
  std::array<hal::byte, BytesToRead> buffer;
  HAL_CHECK(delay(p_serial, BytesToRead));
  HAL_CHECK(p_serial.read(buffer));
  return buffer;
}

/**
 * @brief Perform a write then read transaction over serial.
 *
 * This is especially useful for devices that use a command and response method
 * of communication.
 *
 * @param p_serial - the serial port to have the transaction occur on
 * @param p_data_out - the data to be written to the port
 * @param p_data_in - a buffer to receive the bytes back from the port
 * @return status - return an error if a call to serial::read
 * or serial::write() returns an error from the serial port or success.
 */
[[nodiscard]] inline status write_then_read(
  serial& p_serial,
  std::span<const hal::byte> p_data_out,
  std::span<hal::byte> p_data_in) noexcept
{
  HAL_CHECK(write(p_serial, p_data_out));
  HAL_CHECK(read(p_serial, p_data_in));
  return {};
}

/**
 * @brief Perform a write then read transaction over serial.
 *
 * This is especially useful for devices that use a command and response method
 * of communication.
 *
 * @tparam BytesToRead - the number of bytes to read back
 * @param p_serial - the serial port to have the transaction occur on
 * @param p_data_out - the data to be written to the port
 * @return result<std::array<hal::byte, BytesToRead>> - return an
 * error if a call to serial::read or serial::write() returns an error from the
 * serial port or an array of read bytes.
 */
template<size_t BytesToRead>
[[nodiscard]] result<std::array<hal::byte, BytesToRead>> write_then_read(
  serial& p_serial,
  std::span<const hal::byte> p_data_out) noexcept
{
  std::array<hal::byte, BytesToRead> buffer;
  HAL_CHECK(write_then_read(p_serial, p_data_out, buffer));
  return buffer;
}
/** @} */
}  // namespace hal
