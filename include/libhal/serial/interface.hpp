#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

#include "../error.hpp"
#include "../units.hpp"

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
 * Due to the asynchronous and unformatted nature of serial communication
 * protocols, all implementations of serial devices must be buffered. Buffered,
 * in this case, is defined as automatic storage of received bytes without
 * direct application intervention.
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
    hertz baud_rate = 115200.0f;
    /// Number of stop bits for each frame
    stop_bits stop = stop_bits::one;
    /// Parity bit type for each frame
    parity parity = parity::none;

    /**
     * @brief Default operators for <, <=, >, >= and ==
     *
     * @return auto - result of the comparison
     */
    [[nodiscard]] constexpr auto operator<=>(const settings&) const noexcept =
      default;

    /**
     * @brief Operators ==
     *
     * @return auto - result of the comparison
     */
    [[nodiscard]] constexpr auto operator==(
      const settings& p_settings) const noexcept
    {
      return stop == p_settings.stop && parity == p_settings.parity &&
             equals(baud_rate, p_settings.baud_rate);
    }
  };

  /// Structure informing the caller of the number of bytes that can be read out
  /// and the storage capacity of the serial port.
  struct bytes_available_t
  {
    /// Bytes currently received, enqueued, and available to be read out.
    ///
    /// This value can be equal to or exceed the value of capacity. In this
    /// situation, the number of bytes above the capacity are bytes that have
    /// been dropped. Not all drivers will indicate the number of bytes lost. It
    /// is  up to the driver or application to decide what to do in this
    /// situation.
    size_t available;
    /// The maximum number of bytes that the serial port can queue up.
    size_t capacity;
  };

  /**
   * @brief Configure serial to match the settings supplied
   *
   * Implementing drivers must verify if the settings can be applied to hardware
   * before modifying the hardware. This will ensure that if this operation
   * fails, the state of the serial device has not changed.
   *
   * @param p_settings - settings to apply to serial driver
   * @return status - success or failure
   * @throws std::errc::invalid_argument if the settings could not be achieved
   */
  [[nodiscard]] status configure(const settings& p_settings) noexcept
  {
    return driver_configure(p_settings);
  }

  /**
   * @brief Write data to the transmitter line of the serial port
   *
   * @param p_data - data to be transmitted over the serial port
   * @return result<size_t> - the number of bytes transmitted
   */
  [[nodiscard]] result<size_t> write(std::span<const hal::byte> p_data) noexcept
  {
    return driver_write(p_data);
  }
  /**
   * @brief The number of bytes that have been buffered
   *
   * @return bytes_available_t - number of bytes that can be read out
   * of this serial port as well as the capacity of the serial port.
   */
  [[nodiscard]] bytes_available_t bytes_available() noexcept
  {
    return driver_bytes_available();
  }
  /**
   * @brief Copy bytes from working buffer into passed buffer
   *
   * This operation copies the bytes from the serial driver's internal working
   * buffer to the buffer supplied. This call will subtract from the number
   * returned from bytes_available() function.
   *
   * If the length of this buffer is greater than bytes available, then the
   * buffer is filled up to the length returned by bytes_available(). The rest
   * of the buffer is left untouched.
   *
   * If a frame error has occurred at any point during serial reception, this
   * function will throw a `std::errc::io_error` value. The contents of the
   * internal working buffer as well as the values for bytes_available will stay
   * the same with no copy into the supplied buffered taking place. The frame
   * error will be internally cleared after its occurrence. A subsequent call to
   * read() will read out the contents of the buffer although the data inside
   * may be corrupt. Options are to flush the buffer and attempt reception again
   * or read out the potentially corrupted data and parse it as needed. The
   * choice of operations here are very application and driver specific.
   *
   * @param p_data - Buffer to read bytes back from
   * @return result<std::span<hal::byte>> - The address will
   * ALWAYS be the same as p_data and the length will be equal to the number of
   * bytes read from the buffer.
   * @throws std::errc::io_error - a frame error occurred at some point during
   * reception.
   */
  [[nodiscard]] result<std::span<hal::byte>> read(
    std::span<hal::byte> p_data) noexcept
  {
    return driver_read(p_data);
  }
  /**
   * @brief Flush working buffer
   *
   * The behavior of flushing the internal working buffer is this:
   *
   * - Set the internal circular buffer implementation to an "empty" state.
   * - After this call, so long as data is not received at the same time,
   *   bytes_available() will report to zero bytes
   * - Will clear any received data stored in hardware registers.
   * - Will ensure that this operation is as fast as possible, meaning it will
   *   not zero out the contents of the internal working buffer.
   *
   * @return status - success or failure
   */
  [[nodiscard]] status flush() noexcept
  {
    return driver_flush();
  }

private:
  virtual status driver_configure(const settings& p_settings) noexcept = 0;
  virtual result<size_t> driver_write(
    std::span<const hal::byte> p_data) noexcept = 0;
  virtual bytes_available_t driver_bytes_available() noexcept = 0;
  virtual result<std::span<hal::byte>> driver_read(
    std::span<hal::byte> p_data) noexcept = 0;
  virtual status driver_flush() noexcept = 0;
};
/** @} */
}  // namespace hal
