#pragma once

#include <optional>
#include <span>
#include <string_view>

#include "../as_bytes.hpp"
#include "../error.hpp"
#include "../timeout.hpp"
#include "../units.hpp"
#include "interface.hpp"

namespace hal {
/**
 * @addtogroup serial
 * @{
 */
/**
 * @brief Write bytes to a serial port
 *
 * @param p_serial - the serial port that will be written to
 * @param p_data_out - the data to be written out the port
 * @return result<serial::write_t> - get the results of the uart port write
 * operation.
 */
[[nodiscard]] inline result<serial::write_t> write_partial(
  serial& p_serial,
  std::span<const hal::byte> p_data_out) noexcept
{
  return p_serial.write(p_data_out);
}

/**
 * @brief Write bytes to a serial port
 *
 * @param p_serial - the serial port that will be written to
 * @param p_data_out - the data to be written out the port
 * @return status - success or failure
 */
[[nodiscard]] inline status write(
  serial& p_serial,
  std::span<const hal::byte> p_data_out) noexcept
{
  auto write_info = HAL_CHECK(p_serial.write(p_data_out));

  while (write_info.remaining.size() != 0) {
    write_info = HAL_CHECK(p_serial.write(write_info.remaining));
  }

  return success();
}

/**
 * @brief Write std::span of const char to a serial port
 *
 * @param p_serial - the serial port that will be written to
 * @param p_data_out - chars to be written out the port
 * @return status - success or failure
 */
[[nodiscard]] inline status write(serial& p_serial,
                                  std::span<const char> p_data_out) noexcept
{
  return write(p_serial, hal::as_bytes(p_data_out));
}

/**
 * @brief Write std::span of const char to a serial port
 *
 * @param p_serial - the serial port that will be written to
 * @param p_data_out - chars to be written out the port
 * @return status - success or failure
 */
[[nodiscard]] inline status write(serial& p_serial,
                                  std::string_view p_data_out) noexcept
{
  return write(p_serial, hal::as_bytes(p_data_out));
}

/**
 * @brief Read bytes from a serial port
 *
 * @param p_serial - the serial port that will be read from
 * @param p_data_in - buffer to have bytes from the serial port read into
 * @param p_timeout - timeout callable that indicates when to bail out of the
 * read operation.
 * @return result<std::span<hal::byte>> - return an error if
 * a call to serial::read or delay() returns an error from the serial port or
 * a span with the number of bytes read and a pointer to where the read bytes
 * are.
 */
[[nodiscard]] inline result<std::span<hal::byte>> read(
  serial& p_serial,
  std::span<hal::byte> p_data_in,
  timeout auto p_timeout) noexcept
{
  auto read_result = HAL_CHECK(p_serial.read(p_data_in));

  while (read_result.remaining.size() != 0) {
    read_result = HAL_CHECK(p_serial.read(read_result.remaining));
    HAL_CHECK(p_timeout());
  }

  return p_data_in;
}

/**
 * @brief Read bytes from a serial port and return an array.
 *
 * This call eliminates the boiler plate of creating an array and then passing
 * that to the read function.
 *
 * @tparam BytesToRead - the number of bytes to be read from the serial port.
 * @param p_serial - the serial port to be read from
 * @param p_timeout - timeout callable that indicates when to bail out of the
 * read operation.
 * @return result<std::array<hal::byte, BytesToRead>> - return an
 * error if a call to serial::read or delay() returns an error from the
 * serial port or a span with the number of bytes read and a pointer to where
 * the read bytes are.
 */
template<size_t BytesToRead>
[[nodiscard]] result<std::array<hal::byte, BytesToRead>> read(
  serial& p_serial,
  timeout auto p_timeout) noexcept
{
  std::array<hal::byte, BytesToRead> buffer;
  HAL_CHECK(read(p_serial, buffer, p_timeout));
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
 * @param p_timeout - timeout callable that indicates when to bail out of the
 * read operation.
 * @return status - success or failure
 * or serial::write() returns an error from the serial port or success.
 */
[[nodiscard]] inline result<std::span<hal::byte>> write_then_read(
  serial& p_serial,
  std::span<const hal::byte> p_data_out,
  std::span<hal::byte> p_data_in,
  timeout auto p_timeout) noexcept
{
  HAL_CHECK(write_partial(p_serial, p_data_out));
  return read(p_serial, p_data_in, p_timeout);
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
 * @param p_timeout - timeout callable that indicates when to bail out of the
 * read operation.
 * @return result<std::array<hal::byte, BytesToRead>> - return an
 * error if a call to serial::read or serial::write() returns an error from the
 * serial port or an array of read bytes.
 */
template<size_t BytesToRead>
[[nodiscard]] result<std::array<hal::byte, BytesToRead>> write_then_read(
  serial& p_serial,
  std::span<const hal::byte> p_data_out,
  timeout auto p_timeout) noexcept
{
  std::array<hal::byte, BytesToRead> buffer;
  HAL_CHECK(write_then_read(p_serial, p_data_out, buffer, p_timeout));
  return buffer;
}
/** @} */
}  // namespace hal
