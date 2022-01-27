#pragma once

#include <cinttypes>
#include <cstddef>
#include <span>
#include <system_error>

#include "../driver.hpp"
#include "../frequency.hpp"
#include "../percent.hpp"

namespace embed {
/**
 * @brief Generic settings for a standard I2C device
 *
 */
struct i2c_settings
{
  /// @brief The serial clock rate in hertz.
  frequency clock_rate = default_clock_rate;
};

/**
 * @brief Inter-integrated Circuit (I2C) or Two Wire Interface (TWI)
 * communication protocol hardware abstract interface.
 *
 */
class i2c : public driver<i2c_settings>
{
public:
  /**
   * @brief Error type indicating that the i2c transaction resulted in a NACK,
   * meaning "not acknowledge". NACKs occur when an address has been placed on
   * the i2c bus and no device connected to the bus returned an acknowledge
   * signal.
   *
   * <b>How to handle these errors:</b>
   *
   * - In cases where the program is scanning for available addresses, this
   *   error is expected behaviour. This is less of an error to handle but an
   *   expected return status.
   *
   * - If a driver has a set of possible addresses that it can use, and for that
   *   driver it makes sense to attempt to search for the valid address, then a
   *   handler can keep performing transactions until a valid address is found
   *   and then store that address.
   *
   * - In the case where the address is consistently NACK'ed but the driver
   *   expects a specific address, this is typically not handleable and
   *   indicates that the application or driver is incorrect in the device
   *   address.
   *
   * - In the case where NACK's are spurious, then this may indicate that the
   *   i2c line is faulty or the device is misbehaving. This is not fixable in
   *   the application. But in some cases, if a software solution is required,
   *   the driver or application can simply retry again until a valid response
   *   is returned, but results may vary and are very specific to the devices
   *   and context of the situation.
   *
   */
  struct address_not_acknowledged
  {};

  /**
   * @brief Error type indicating that the i2c lines were put into an invalid
   * state during the transaction due to interference, misconfiguration of the
   * i2c peripheral or the addressed device or something else.
   *
   * <b>How to handle these errors:</b>
   *
   * - In the event of this type of error, state the addressed device undefined.
   *   In the case of data reception, the data coming from the addressed device
   *   should be considered invalid. Any deeper handling will require deep
   *   context regarding the addressed device the transaction taking place.
   *
   */
  struct bus_error
  {};

  /// default constructor
  i2c() = default;
  /// Explicitly delete copy constructor to prevent slicing
  i2c(const i2c& p_other) = delete;
  /// Explicitly delete assignment operator to prevent slicing
  i2c& operator=(const i2c& p_other) = delete;
  /// Destroy the object
  virtual ~i2c() = default;

  /**
   * @brief perform an i2c transaction with another device on the bus. The type
   * of transaction depends on values of input parameters. This function will
   * block until the entire transfer is finished.
   *
   * Performing Write, Read and Write-Then-Read transactions depends on which
   * span for data_out and data_in are set to null.
   *
   * - For write transactions, pass p_data_in as an empty span
   * `std::span<std::byte>{}` and pass a buffer to p_data_out.
   *
   * - For read transactions, pass p_data_out as an empty span
   * `std::span<const std::byte>{}` and pass a buffer to p_data_in.
   *
   * - For write-then-read transactions, pass a buffer for both p_data_in
   * p_data_out.
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
   * nullptr with length zero in order to skip writting.
   * @param p_data_in buffer to store read data from the addressed device. Set
   * to nullptr with length 0 in order to skip reading.
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  virtual boost::leaf::result<void> transaction(
    std::byte p_address,
    std::span<const std::byte> p_data_out,
    std::span<std::byte> p_data_in) = 0;
};
}  // namespace embed
