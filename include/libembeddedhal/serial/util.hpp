/**
 * @file util.hpp
 * @brief Provide utility functions for the serial interface
 */
#pragma once

#include <optional>
#include <span>
#include <string_view>

#include "../counter/interface.hpp"
#include "../error.hpp"
#include "../time.hpp"
#include "interface.hpp"

namespace embed {

/**
 * @brief Create a timeout handler lambda from a serial that satisfies the
 * embed::timeout_handler definition.
 *
 * @param p_serial - hardware serial driver
 * @return auto - lambda timeout handler based on the serial
 */
[[nodiscard]] inline auto to_timeout(serial& p_serial, size_t p_length) noexcept
{
  auto handler = [&p_serial, p_length]() mutable -> boost::leaf::result<bool> {
    if (BOOST_LEAF_CHECK(p_serial.bytes_available()) >= p_length) {
      return true;
    }
    return false;
  };

  static_assert(
    std::is_constructible_v<std::function<timeout_handler>, decltype(handler)>,
    "[INTERNAL] Callable must be convertible to a embed::uptime_handler");

  return handler;
}

/**
 * @brief Create a timeout handler lambda from a serial that satisfies the
 * embed::timeout_handler definition.
 *
 * @param p_serial - hardware serial driver
 * @return auto - lambda timeout handler based on the serial
 */
template<typename TimeoutHandler = std::function<timeout_handler>>
[[nodiscard]] inline auto to_timeout(serial& p_serial,
                                     size_t p_length,
                                     TimeoutHandler p_timeout_handler) noexcept
{
  auto serial_timeout_handler = to_timeout(p_serial, p_length);
  auto handler = [serial_timeout_handler,
                  p_timeout_handler]() -> boost::leaf::result<bool> {
    if (serial_timeout_handler()) {
      return true;
    }
    if (p_timeout_handler()) {
      auto on_error = embed::error::setup();
      return boost::leaf::new_error(embed::error::timeout{});
    }
    return false;
  };

  static_assert(
    std::is_constructible_v<std::function<timeout_handler>, decltype(handler)>,
    "[INTERNAL] Callable must be convertible to a embed::uptime_handler");

  return handler;
}
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
 * @return boost::leaf::result<void> - return an error if a call to
 * serial::bytes_available returns an error from the serial port.
 */
[[nodiscard]] inline boost::leaf::result<void> delay(serial& p_serial,
                                                     size_t p_length) noexcept
{
  size_t bytes_available = BOOST_LEAF_CHECK(p_serial.bytes_available());
  while (bytes_available < p_length) {
    bytes_available = BOOST_LEAF_CHECK(p_serial.bytes_available());
  }
  return {};
}

/**
 * @brief Write bytes to a serial port
 *
 * @param p_serial - the serial port that will be written to
 * @param p_data_out - the data to be written out the port
 * @return boost::leaf::result<void> - return an error if a call to
 * serial::write returns an error from the serial port.
 */
[[nodiscard]] inline boost::leaf::result<void> write(
  serial& p_serial,
  std::span<const std::byte> p_data_out) noexcept
{
  return p_serial.write(p_data_out);
}

/**
 * @brief Read bytes from a serial port
 *
 * @param p_serial - the serial port that will be read from
 * @param p_data_in - buffer to have bytes from the serial port read into
 * @return boost::leaf::result<std::span<const std::byte>> - return an error if
 * a call to serial::read or delay() returns an error from the serial port or
 * a span with the number of bytes read and a pointer to where the read bytes
 * are.
 */
[[nodiscard]] inline boost::leaf::result<std::span<const std::byte>> read(
  serial& p_serial,
  std::span<std::byte> p_data_in)
{
  BOOST_LEAF_CHECK(delay(p_serial, p_data_in.size()));
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
 * @return boost::leaf::result<std::array<std::byte, BytesToRead>> - return an
 * error if a call to serial::read or delay() returns an error from the
 * serial port or a span with the number of bytes read and a pointer to where
 * the read bytes are.
 */
template<size_t BytesToRead>
[[nodiscard]] boost::leaf::result<std::array<std::byte, BytesToRead>> read(
  serial& p_serial) noexcept
{
  std::array<std::byte, BytesToRead> buffer;
  BOOST_LEAF_CHECK(delay(p_serial, BytesToRead));
  BOOST_LEAF_CHECK(p_serial.read(buffer));
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
 * @return boost::leaf::result<void> - return an error if a call to serial::read
 * or serial::write() returns an error from the serial port or success.
 */
[[nodiscard]] inline boost::leaf::result<void> write_then_read(
  serial& p_serial,
  std::span<const std::byte> p_data_out,
  std::span<std::byte> p_data_in) noexcept
{
  BOOST_LEAF_CHECK(write(p_serial, p_data_out));
  BOOST_LEAF_CHECK(read(p_serial, p_data_in));
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
 * @return boost::leaf::result<std::array<std::byte, BytesToRead>> - return an
 * error if a call to serial::read or serial::write() returns an error from the
 * serial port or an array of read bytes.
 */
template<size_t BytesToRead>
[[nodiscard]] boost::leaf::result<std::array<std::byte, BytesToRead>>
write_then_read(serial& p_serial,
                std::span<const std::byte> p_data_out) noexcept
{
  std::array<std::byte, BytesToRead> buffer;
  BOOST_LEAF_CHECK(write_then_read(p_serial, p_data_out, buffer));
  return buffer;
}
}  // namespace embed