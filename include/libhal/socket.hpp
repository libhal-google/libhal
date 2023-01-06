#pragma once

#include <span>

#include "error.hpp"
#include "functional.hpp"
#include "timeout.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief An interface for generic network sockets
 *
 * This socket can represent a TCP or UDP socket for clients or servers.
 * Unlike other socket APIs, this API does not include all of the APIs from the
 * Berkeley Socket API. A description of why the following Berkeley Socket APIs
 * are not included in the interface is as follows:
 *
 * - `socket()`: Initializes a socket object, but this can be done in a
 *               factory and via a constructor. This API makes sense for C
 *               but does not make sense for C++ with RAII.
 * - `bind()`:   For server sockets bind can occur at factory creation time for
 *               the socket, and doesn't need a separate API.
 * - `listen()`: There is no reason for there to be a separate API for listening
 *               for connections and data on the socket as this can also happen
 *               at factory creation time.
 * - `connect()`: For client side sockets, on creation of the socket the socket
 *                should already be connected to the server and be ready to be
 *                used. The socket will be connected at construction and closed
 *                on destruction.
 */
class socket
{
public:
  enum class type : hal::byte
  {
    tcp,
    udp,
    ssl
  };

  /// Return data for the write operation
  struct write_t
  {
    /// The amount of data that was successfully written to the
    std::span<const hal::byte> data;
  };

  /// Return data for the read operation
  struct read_t
  {
    /// The position and amount of bytes that have read into the input buffer
    /// from the socket
    std::span<hal::byte> data;
  };

  /**
   * @brief Write data to the socket
   *
   * @param p_data - data to read to read data from the socket into
   * @param p_timeout - the amount of time to wait for data to be sent. This is
   * necessary for sockets, such as TCP sockets, which have to wait to confirm
   * that the data sent to the server has made it in time.
   * @return hal::result<write_t> - write_t data or error
   * @throw std::errc::no_link if the connection to server has been severed
   * @throw std::errc::timed_out if writing/sending data to the server could not
   * be done in time.
   */
  [[nodiscard]] hal::result<write_t> write(
    std::span<const hal::byte> p_data,
    hal::function_ref<hal::timeout_function> p_timeout)
  {
    return driver_write(p_data, p_timeout);
  }

  /**
   * @brief Read data from the socket
   *
   * Sockets, like serial ports, must be buffered. The buffering can be done
   * by the socket driver itself or can leverage buffers from underlying
   * peripherals such as a serial port, if that is used for socket
   * communication.
   *
   * @param p_data - buffer to read data from the socket into
   * @return hal::result<read_t> - read_t data or error
   * @throw std::errc::no_link if the connection to server has been severed
   */
  [[nodiscard]] hal::result<read_t> read(std::span<hal::byte> p_data)
  {
    return driver_read(p_data);
  }

  /**
   * @brief Destroy the socket client object
   *
   * Attempt to close and release any resources occupied or allocated by the
   * socket. Attempt means, in the process of closing sockets and releasing
   * resources, if errors occur, then the errors should be ignored.
   *
   */
  virtual ~socket() = default;

private:
  virtual hal::result<write_t> driver_write(
    std::span<const hal::byte> p_data,
    hal::function_ref<hal::timeout_function> p_timeout) = 0;
  virtual hal::result<read_t> driver_read(std::span<hal::byte> p_data) = 0;
};
}  // namespace hal
