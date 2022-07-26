#pragma once

#include "../error.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

namespace embed {
/**
 * @addtogroup serial
 * Available serial APIs
 * @{
 */
/**
 * @brief Serial communication protocol hardware abstract interface.
 *
 * Use this interface for hardware that implements a serial protocol like UART,
 * RS232, RS485 and many others that use a similar communication protocol but
 * may use different voltage schemes.
 *
 * Due to the asynchronous nature of serial communication protocols, all
 * implementations of serial devices MUST buffer received bytes using DMA or
 * interrupts. A thread should not be used as this generally greatly increases
 * the ROM and RAM cost of using library.
 *
 * All implementations MUST allow the user to supply their own buffer of
 * arbitrary size, such that the amount of buffered data can be tailored to the
 * needs of the application.
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
    /// Parity bit type for each frame
    parity parity = parity::none;
    /// Number of stop bits for each frame
    stop_bits stop = stop_bits::one;
    /// Number of bits in each frame. Typically between 5 to 9.
    uint8_t frame_size = 8;
  };

  /**
   * @brief Configure serial to match the settings supplied
   *
   * @param p_settings - settings to apply to serial driver
   * @return boost::leaf::result<void>
   * @throws std::errc::invalid_argument if the settings could not be achieved.
   */
  [[nodiscard]] boost::leaf::result<void> configure(
    const settings& p_settings) noexcept
  {
    return driver_configure(p_settings);
  }

  /**
   * @brief Write data on the transmitter line of the port. This function will
   * block until the entire transfer is finished.
   *
   * Data frames are not compact when frame size is less than 8 bits. Meaning
   * that, if you want to send three 5-bit frames, then you will need to use a
   * span of at least 3 bytes to hold each value.
   *
   * When writing data with frame size greater than 8 is in little endian order.
   * Meaning that the first byte in the sequence is the lower byte and the next
   * is the greater byte. If you wanted to send a 9-bit frame with value 0x14A,
   * the first byte must be 0x4A and the next 0x01.
   *
   * serial ports will report `transmit_error` will an error occurs. Any other
   * reported error will bubble up come from
   *
   * @param p_data - data to be transmitted over the serial port transmitter
   * line
   * @return boost::leaf::result<void>
   */
  [[nodiscard]] boost::leaf::result<void> write(
    std::span<const std::byte> p_data) noexcept
  {
    return driver_write(p_data);
  }
  /**
   * @brief The number of bytes that have been buffered. For frames less than
   * 8-bits, each byte holds a frame. For frames above 8-bits the number of
   * bytes returned indicates the number of bytes a buffer needs to be to return
   * all of the data held in the buffer.
   *
   * @return boost::leaf::result<size_t> - number of buffered by the serial
   * driver and are available to be read by the read() function.
   * @throws protocol_error indicates that a parity error occurred during
   * reception.
   * @throws io_error indicates that a frame error occurred during reception.
   * This error is returned when calling bytes_available().
   */
  [[nodiscard]] boost::leaf::result<size_t> bytes_available() noexcept
  {
    return driver_bytes_available();
  }
  /**
   * @brief Read the bytes received over the ports receiver line and stored in
   * the serial implementations buffer. The number of bytes read will subtract
   * the number of bytes available until it reaches zero.
   *
   * @param p_data - Buffer to read bytes back from. If the length of this
   * buffer is greater than the value returned by bytes_available() then buffer
   * is filled up to the length returned by bytes_available(). The rest of the
   * buffer is left untouched.
   * @return boost::leaf::result<std::span<const std::byte>> - provides a means
   * to get the length of bytes read into the buffer p_data. The address will
   * ALWAYS be the same as p_data and the length will be equal to the number of
   * bytes read from the buffer.
   */
  [[nodiscard]] boost::leaf::result<std::span<const std::byte>> read(
    std::span<std::byte> p_data) noexcept
  {
    return driver_read(p_data);
  }
  /**
   * @brief Set bytes_available() to zero and clear any received data stored in
   * hardware registers.
   *
   * @return boost::leaf::result<void>
   */
  [[nodiscard]] boost::leaf::result<void> flush() noexcept
  {
    return driver_flush();
  }

private:
  virtual boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept = 0;
  virtual boost::leaf::result<void> driver_write(
    std::span<const std::byte> p_data) noexcept = 0;
  virtual boost::leaf::result<size_t> driver_bytes_available() noexcept = 0;
  virtual boost::leaf::result<std::span<const std::byte>> driver_read(
    std::span<std::byte> p_data) noexcept = 0;
  virtual boost::leaf::result<void> driver_flush() noexcept = 0;
};
/** @} */
}  // namespace embed
