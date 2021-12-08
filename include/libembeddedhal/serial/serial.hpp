#pragma once

#include "../driver.hpp"

#include <cinttypes>
#include <cstddef>
#include <span>

namespace embed {
/**
 * @brief Generic settings for a standard serial device.
 *
 */
struct serial_settings
{
  /**
   * @brief Set of available stop bits options
   *
   */
  enum class stop_bits : uint8_t
  {
    one = 0,
    two,
  };

  /**
   * @brief Set of parity bit options
   *
   */
  enum class parity : uint8_t
  {
    /**
     * @brief Disable parity bit as part of the frame
     *
     */
    none = 0,
    /**
     * @brief Enable parity and set 1 (HIGH) when the number of bits is odd
     *
     */
    odd,
    /**
     * @brief Enable parity and set 1 (HIGH) when the number of bits is even
     *
     */
    even,
    /**
     * @brief Enable parity bit and always return 1 (HIGH) for ever frame
     *
     */
    forced1,
    /**
     * @brief Enable parity bit and always return 0 (LOW) for ever frame
     *
     */
    forced0,
  };

  /**
   * @brief The operating speed of the baud rate (in units of bits per second)
   *
   */
  uint32_t baud_rate = 115200;
  /**
   * @brief Parity bit type for each frame
   *
   */
  parity parity = parity::none;
  /**
   * @brief Number of stop bits for each frame
   *
   */
  stop_bits stop = stop_bits::one;
  /**
   * @brief Number of bits in each frame. Typically between 5 to 9.
   *
   */
  uint8_t frame_size = 8;
};

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
class serial : public driver<serial_settings>
{
public:
  /**
   * @brief Write data on the transmitter line of the port. The exact
   * implementation for this can be polling, interrupt driven or DMA driven.
   * There is no guarantee on exactly how a write operation will operate, thus
   * the busy() function must polled called after this returns to determine if
   * the write operation has finished. Attempting to call write() when
   * tranmission is busy is undefined behavior.
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
   * @param p_data data to be transmitted over the serial port transmitter line
   */
  virtual void write(std::span<const std::byte> p_data) = 0;
  /**
   * @brief Determines if the write operation is currently on going.
   *
   * @return true serial transmitter is currently writing data to the port
   * @return false serial transmitter is idle and  available to write data
   */
  [[nodiscard]] virtual bool busy() = 0;
  /**
   * @brief The number of bytes that have been buffered. For frames less than
   * 8-bits, each byte holds a frame. For frames above 8-bits the number of
   * bytes returned indicates the number of bytes a buffer needs to be to return
   * all of the data held in the buffer.
   *
   * @return size_t number of buffered by the serial driver and are available to
   * be read by the read() function.
   */
  [[nodiscard]] virtual size_t bytes_available() = 0;
  /**
   * @brief Read the bytes received over the ports receiver line and stored in
   * the serial implementations buffer. The number of bytes read will subtract
   * the number of bytes available until it reaches zero.
   *
   * @param p_data Buffer to read bytes back from. If the length of this buffer
   * is greater than the value returned by bytes_available() then buffer is
   * filled up to the length returned by bytes_available(). The rest of the
   * buffer is left untouched.
   * @return std::span<const std::byte> provides a means to get the length of
   * bytes read into the buffer p_data. The address will ALWAYS be the same as
   * p_data and the length will be equal to the number of bytes read from the
   * buffer.
   */
  virtual std::span<const std::byte> read(std::span<std::byte> p_data) = 0;
  /**
   * @brief Set bytes_available() to zero and clear any received data stored in
   * hardware registers. This operation must be faster than simply running
   * read() until bytes_available() is empty.
   *
   */
  virtual void flush() = 0;
};
}  // namespace embed
