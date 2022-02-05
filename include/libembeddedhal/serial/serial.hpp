#pragma once

#include "../error.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

namespace embed {
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
   * @brief Error indicating that packets were lost during reception. This
   * occurs when the buffer is overrun and reaches the end of the circular
   * buffer. This error is returned when calling bytes_available().
   *
   * <b>How to handle these errors:</b>
   *
   * - This sort of error is very context heavy. If the packet lengths are short
   *   and numerous and with a consistent format, then the application may be
   *   able to pull out all but the last few packets of data from the buffer and
   *   treat the ones at the end as lost packets.
   *
   * - In most other cases where data lost was crucial, then the whole buffer
   *   may need to be flushed and the data received again.
   *
   * - A way to fix this is to enlarge the receive buffer. This can be done with
   *   dynamic memory allocation, but generally, its better to simply increase
   *   the buffer size by updating the application rather than growing with
   *   need.
   *
   */
  struct packets_lost
  {
    /// The number of packets dropped. This value is optional because some
    /// drivers cannot provide a number for the number of packets dropped.
    std::optional<size_t> packets_dropped;
    /// The number of bytes the serial port has buffered up.
    size_t bytes_available;
  };

  /**
   * @brief Error type indicating that a frame error occurred during reception.
   * This error is returned when calling bytes_available().
   *
   * <b>How to handle these errors:</b>
   *
   * - In general, the exact nature of a specific frame error is not knowable.
   *   The number of bytes that are effected could be 1 or more, thus the only
   *   real way to handle this is to flush receive buffer and attempt reception
   *   again. Note that the read function should still work to read out which
   *   ever bytes were received and this can be used/logged in order for
   *   developers to get insight into where the error occurred and how to fix
   * it.
   *
   */
  struct frame_error
  {
    /// The number of bytes the serial port has buffered up.
    size_t bytes_available;
  };
  /**
   * @brief Error type indicating that a parity error occurred during reception.
   * This error is returned when calling bytes_available().
   *
   * <b>How to handle these errors:</b>
   *
   * - The nature of this error is almost exactly the same as frame_error. See
   *   frame_error's description on how to handle this.
   *
   */
  struct parity_error
  {
    /// The number of bytes the serial port has buffered up.
    size_t bytes_available;
  };

  /**
   * @brief Configure serial to match the settings supplied
   *
   * @param p_settings - settings to apply to serial driver
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation. Will return embed::error::invalid_settings if the settings could
   * not be achieved.
   */
  [[nodiscard]] boost::leaf::result<void> configure(const settings& p_settings)
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
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  [[nodiscard]] boost::leaf::result<void> write(
    std::span<const std::byte> p_data)
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
   */
  [[nodiscard]] boost::leaf::result<size_t> bytes_available()
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
    std::span<std::byte> p_data)
  {
    return driver_read(p_data);
  }
  /**
   * @brief Set bytes_available() to zero and clear any received data stored in
   * hardware registers.
   *
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  [[nodiscard]] boost::leaf::result<void> flush() { return driver_flush(); }

private:
  virtual boost::leaf::result<void> driver_configure(
    const settings& p_settings) = 0;
  virtual boost::leaf::result<void> driver_write(
    std::span<const std::byte> p_data) = 0;
  virtual boost::leaf::result<size_t> driver_bytes_available() = 0;
  virtual boost::leaf::result<std::span<const std::byte>> driver_read(
    std::span<std::byte> p_data) = 0;
  virtual boost::leaf::result<void> driver_flush() = 0;
};
}  // namespace embed
