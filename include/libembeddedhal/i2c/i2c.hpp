#pragma once

#include "../driver.hpp"
#include "../full_scale.hpp"

#include <cinttypes>
#include <cstddef>
#include <span>

namespace embed {
/**
 * @brief Generic settings for a standard I2C device
 *
 */
struct i2c_settings
{
  /**
   * @brief The serial clock rate in hertz.
   *
   */
  uint32_t clock_rate_hz = 100'000;
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
   * @brief perform an i2c transaction with another device on the bus. The type
   * of transaction depends on values of input parameters.
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
   * Implementations of transaction can be synchronous, interrupt driven or DMA
   * driven. Calling transaction while a current transaction is ongoing is
   * undefined behavior. To prevent this, poll busy() until it returns false.
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
   * @return boost::leaf::result<void> - any errors that occured during this
   * operation.
   */
  virtual boost::leaf::result<void> transaction(
    std::byte p_address,
    std::span<const std::byte> p_data_out,
    std::span<std::byte> p_data_in) = 0;
  /**
   * @brief Determines if the i2c transaction is ongoing
   *
   * @return boost::leaf::result<bool> - true if a transaction is currently
   * ongoing, false if otherwise.
   */
  virtual boost::leaf::result<bool> busy() = 0;
};
}  // namespace embed
