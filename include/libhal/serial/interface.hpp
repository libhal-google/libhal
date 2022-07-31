#pragma once

#include "../error.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

namespace hal {
/**
 * @addtogroup serial
 * Available serial APIs
 * @{
 */
/**
 * @brief Hardware abstract interface for the serial communication protocol
 *
 * Use this interface for hardware that implements a serial protocol like UART,
 * RS232, RS485 and others that use a similar communication protocol but may use
 * different voltage schemes.
 *
 * This interface only works 8-bit serial data frames.
 *
 * Due to the asynchronous nature of serial communication protocols, all
 * implementations of serial devices must be buffered. Buffered, in this case,
 * is defined as automatic storage of received bytes without direct application
 * intervention.
 *
 * All implementations MUST allow the user to supply their own buffer of
 * arbitrary size up to the limits of what hardware can support. This allows a
 * developer the ability to tailored the buffer size to the needs of the
 * application.
 *
 * Examples of buffering schemes are:
 *
 * - Using DMA to copy data from a serial peripheral to a region of memory
 * - Using interrupts when a serial peripheral's queue has filled to a point
 *
 */
class serial
{
public:
  /// Generic settings for a standard serial device.
  struct settings
  {
    /// Set of available stop bits options
    enum class stop_bits : uint8_t
    {
      one = 0,
      two,
    };

    /// Set of parity bit options
    enum class parity : uint8_t
    {
      /// Disable parity bit as part of the frame
      none = 0,
      /// Enable parity and set 1 (HIGH) when the number of bits is odd
      odd,
      /// Enable parity and set 1 (HIGH) when the number of bits is even
      even,
      /// Enable parity bit and always return 1 (HIGH) for ever frame
      forced1,
      /// Enable parity bit and always return 0 (LOW) for ever frame
      forced0,
    };

    /// The operating speed of the baud rate (in units of bits per second)
    uint32_t baud_rate = 115200;
    /// Number of stop bits for each frame
    stop_bits stop = stop_bits::one;
    /// Parity bit type for each frame
    parity parity = parity::none;
  };

  /**
   * @brief Configure serial to match the settings supplied
   *
   * Implementing drivers must verify if the settings can be applied to hardware
   * before modifying the hardware. This will ensure that if this operation
   * fails, the state of the serial device has not changed.
   *
   * @param p_settings - settings to apply to serial driver
   * @return status
   * @throws std::errc::invalid_argument if the settings could not be achieved
   */
  [[nodiscard]] status configure(const settings& p_settings) noexcept
  {
    return driver_configure(p_settings);
  }

  /**
   * @brief Write data on the transmitter line of the port
   *
   * This function will block until the entire transfer is finished.
   *
   * @param p_data - data to be transmitted over the serial port
   * @return status
   */
  [[nodiscard]] status write(std::span<const std::byte> p_data) noexcept
  {
    return driver_write(p_data);
  }
  /**
   * @brief The number of bytes that have been buffered
   *
   * @return result<size_t> - number of bytes that can be read out
   * of this serial port.
   * @throws std::errc::protocol_error indicates that a parity error occurred
   * during reception.
   * @throws std::errc::io_error indicates that a frame error occurred during
   * reception.
   */
  [[nodiscard]] result<size_t> bytes_available() noexcept
  {
    return driver_bytes_available();
  }
  /**
   * @brief Read the bytes received over the receiver line into buffer
   *
   * This operation copies the bytes from the serial driver's internal buffer to
   * the buffer supplied. This call will subtract from the number returned from
   * bytes_available() function.
   *
   * @param p_data - Buffer to read bytes back from. If the length of this
   * buffer is greater than bytes available, then the buffer is filled up to the
   * length returned by bytes_available(). The rest of the buffer is left
   * untouched.
   * @return result<std::span<const std::byte>> - The address will
   * ALWAYS be the same as p_data and the length will be equal to the number of
   * bytes read from the buffer.
   */
  [[nodiscard]] result<std::span<const std::byte>> read(
    std::span<std::byte> p_data) noexcept
  {
    return driver_read(p_data);
  }
  /**
   * @brief Set bytes_available() to zero and clear any received data stored in
   * hardware registers.
   *
   * @return status
   */
  [[nodiscard]] status flush() noexcept
  {
    return driver_flush();
  }

private:
  virtual status driver_configure(const settings& p_settings) noexcept = 0;
  virtual status driver_write(std::span<const std::byte> p_data) noexcept = 0;
  virtual result<size_t> driver_bytes_available() noexcept = 0;
  virtual result<std::span<const std::byte>> driver_read(
    std::span<std::byte> p_data) noexcept = 0;
  virtual status driver_flush() noexcept = 0;
};
/** @} */
}  // namespace hal
