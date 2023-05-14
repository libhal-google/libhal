// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

#include "error.hpp"
#include "units.hpp"

namespace hal {
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
  /**
   * @brief Generic settings for a standard serial device.
   *
   */
  struct settings
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
       * @brief Enable parity bit and always return 1 (HIGH) for ever
       * frame
       *
       */
      forced1,
      /**
       * @brief Enable parity bit and always return 0 (LOW) for ever
       * frame
       *
       */
      forced0,
    };

    /**
     * @brief The operating speed of the baud rate (in units of bits per second)
     *
     */
    hertz baud_rate = 115200.0f;

    /**
     * @brief Number of stop bits for each frame
     *
     */
    stop_bits stop = stop_bits::one;

    /**
     * @brief Parity bit type for each frame
     *
     */
    parity parity = parity::none;
  };
  /**
   * @brief Return type for serial read operations
   *
   */
  struct read_t
  {
    /**
     * @brief  The filled portion of the input buffer from the serial port
     *
     * The size of this buffer indicates the number of bytes read The address
     * points to the start of the buffer passed into the read() function.
     */
    std::span<hal::byte> data;

    /**
     * @brief Number of enqueued and available to be read out bytes
     *
     * This value can be equal to or exceed the value of capacity. In this
     * situation, the number of bytes above the capacity are bytes that have
     * been dropped. Not all drivers will indicate the number of bytes lost. It
     * is up to the driver or application to decide what to do in this
     * situation.
     */
    size_t available;

    /**
     * @brief The maximum number of bytes that the serial port can queue up.
     *
     */
    size_t capacity;
  };

  /**
   * @brief Return type for serial write operations
   *
   */
  struct write_t
  {
    /**
     * @brief The portion of the buffer transmitted
     *
     */
    std::span<const hal::byte> data;
  };

  /**
   * @brief Feedback from performing a flush operation
   *
   * This structure is currently empty as no feedback has been determined for
   * now. This structure may be expanded in the future.
   */
  struct flush_t
  {};

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
  [[nodiscard]] status configure(const settings& p_settings)
  {
    return driver_configure(p_settings);
  }

  /**
   * @brief Write data to the transmitter line of the serial port
   *
   * @param p_data - data to be transmitted over the serial port
   * @return result<write_t> - serial write response
   */
  [[nodiscard]] result<write_t> write(std::span<const hal::byte> p_data)
  {
    return driver_write(p_data);
  }

  /**
   * @brief Copy bytes from working buffer into passed buffer
   *
   * This operation copies the bytes from the serial driver's internal working
   * buffer to the buffer supplied.
   *
   * The buffer will be filled up either to the end of the buffer or until there
   * are no more bytes left in the working buffer. The remaining portion of the
   * input buffer is returned in `read_t::remaining`.
   *
   * If a frame error has occurred at any point during serial reception, this
   * function will throw a `std::errc::io_error` value. The contents of the
   * internal working buffer will stay the same. No information from the
   * internal working buffer will be copied into the supplied buffer and no data
   * will be removed from the internal working buffer. The frame error status
   * will be internally cleared after its occurrence. Subsequent calls of this
   * function will read out the contents of the buffer although the data inside
   * may be corrupt.
   *
   * When an error occurs the options available are to flush the buffer and
   * attempt reception again or read out the potentially corrupted data and
   * parse it as needed. The choice of operation is application/driver specific.
   *
   * @param p_data - Buffer to read bytes in to
   * @return result<read_t> - serial read response data
   * @throws std::errc::io_error - a frame error occurred at some point during
   * reception.
   */
  [[nodiscard]] result<read_t> read(std::span<hal::byte> p_data)
  {
    return driver_read(p_data);
  }
  /**
   * @brief Flush working buffer
   *
   * The behavior of flushing the internal working buffer is this:
   *
   * - Sets the serial port's internal working buffer to an "empty" state.
   * - Clear any received data stored in hardware registers.
   * - Use the fastest available option to perform these operations, meaning
   *   that the contents of the internal working buffer will not be zeroed out.
   *
   * @return result<flush_t> - success or failure
   */
  [[nodiscard]] result<flush_t> flush()
  {
    return driver_flush();
  }

  virtual ~serial() = default;

private:
  virtual status driver_configure(const settings& p_settings) = 0;
  virtual result<write_t> driver_write(std::span<const hal::byte> p_data) = 0;
  virtual result<read_t> driver_read(std::span<hal::byte> p_data) = 0;
  virtual result<flush_t> driver_flush() = 0;
};
}  // namespace hal
