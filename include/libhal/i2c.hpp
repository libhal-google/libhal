#pragma once

#include <functional>
#include <span>

#include "error.hpp"
#include "timeout.hpp"
#include "units.hpp"

namespace hal {
/**
 * @addtogroup i2c
 * Available Inter-integrated Circuit (I2C) APIs
 * @{
 */
/**
 * @brief Inter-integrated Circuit (I2C) hardware abstract interface.
 *
 * Also known as Two Wire Interface (TWI) communication protocol. This is a very
 * commonly used protocol for communication with sensors and peripheral devices
 * because it only requires two connections SDA (data signal) and SCL (clock
 * signal). This is possible because the protocol for I2C is addressable.
 *
 */
class i2c
{
public:
  /**
   * @brief Generic settings for a standard I2C device
   *
   */
  struct settings
  {
    /// @brief The serial clock rate in hertz.
    hertz clock_rate = 100.0_kHz;
  };

  /**
   * @brief Configure i2c to match the settings supplied
   *
   * @param p_settings - settings to apply to i2c driver
   * @return status - success or failure
   * @throws std::errc::invalid_argument if the settings could not be achieved.
   */
  [[nodiscard]] status configure(const settings& p_settings) noexcept
  {
    return driver_configure(p_settings);
  }

  /**
   * @brief perform an i2c transaction with another device on the bus. The type
   * of transaction depends on values of input parameters. This function will
   * block until the entire transfer is finished.
   *
   * Performing Write, Read and Write-Then-Read transactions depends on which
   * span for data_out and data_in are set to null.
   *
   * - For write transactions, pass p_data_in as an empty span
   * `std::span<hal::byte>{}` and pass a buffer to p_data_out.
   *
   * - For read transactions, pass p_data_out as an empty span
   * `std::span<const hal::byte>{}` and pass a buffer to p_data_in.
   *
   * - For write-then-read transactions, pass a buffer for both p_data_in
   *   p_data_out.
   *
   * - If both p_data_in and p_data_out are empty, simply do nothing and return
   *   success.
   *
   * In the event of arbitration loss, this function will wait for the bus to
   * become free and try again. Arbitration loss means that during the address
   * phase of a transaction 1 or more i2c bus controllers attempted to perform
   * an transaction and one of the i2c bus controllers, that isn't this one won
   * out.
   *
   * @param p_address 7-bit address of the device you want to communicate with.
   * To perform a transaction with a 10-bit address, this parameter must be the
   * address upper byte of the 10-bit address OR'd with 0b1111'0000 (the 10-bit
   * address indicator). The lower byte of the address must be contained in the
   * first byte of the p_data_out span.
   * @param p_data_out data to be written to the addressed device. Set to
   * nullptr with length zero in order to skip writing.
   * @param p_data_in buffer to store read data from the addressed device. Set
   * to nullptr with length 0 in order to skip reading.
   * @param p_timeout callable which notifies the i2c driver that it has run out
   * of time to perform the transaction and must stop and return control to the
   * caller.
   * @return status - success or failure
   * @throws std::errc::io_error indicates that the i2c lines were put into an
   * invalid state during the transaction due to interference, misconfiguration
   * of the i2c peripheral or the addressed device or something else.
   * @throws std::errc::no_such_device_or_address indicates that no devices on
   * the bus acknowledge the address in this transaction, which could mean that
   * the device is not connected to the bus, is not powered, not available to
   * respond, broken or many other possible outcomes.
   * @throws std::errc::timed_out if the transaction exceeded its time allotment
   * indicated by p_timeout.
   */
  [[nodiscard]] status transaction(
    hal::byte p_address,
    std::span<const hal::byte> p_data_out,
    std::span<hal::byte> p_data_in,
    std::function<hal::timeout_function> p_timeout) noexcept
  {
    return driver_transaction(p_address, p_data_out, p_data_in, p_timeout);
  }

  virtual ~i2c() = default;

private:
  virtual status driver_configure(const settings& p_settings) noexcept = 0;
  virtual status driver_transaction(
    hal::byte p_address,
    std::span<const hal::byte> p_data_out,
    std::span<hal::byte> p_data_in,
    std::function<hal::timeout_function> p_timeout) noexcept = 0;
};
/** @} */
}  // namespace hal
