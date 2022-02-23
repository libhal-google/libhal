#pragma once

#include <span>

#include "../error.hpp"
#include "spi.hpp"

namespace embed {
/**
 * @brief Write data to the SPI bus and ignore data sent from peripherals on the
 * bus.
 *
 * @param p_spi - spi driver
 * @param p_data_out - data to be written to the SPI bus
 * @return boost::leaf::result<void> - any errors associated with this call
 */
[[nodiscard]] inline boost::leaf::result<void> write(
  spi& p_spi,
  std::span<const std::byte> p_data_out) noexcept
{
  return p_spi.transfer(p_data_out, std::span<std::byte>{}, std::byte{ 0xFF });
}

/**
 * @brief Read data from the SPI bus.
 *
 * Filler bytes will be placed on the write line.
 *
 * @param p_spi - spi driver
 * @param p_data_in - buffer to receive bytes back from the SPI bus
 * @param p_filler - filler data placed on the bus in place of actual write
 * data.
 * @return boost::leaf::result<void> - any errors associated with this call
 */
[[nodiscard]] inline boost::leaf::result<void> read(
  spi& p_spi,
  std::span<std::byte> p_data_in,
  std::byte p_filler = std::byte{ 0xFF }) noexcept
{
  return p_spi.transfer(std::span<std::byte>{}, p_data_in, p_filler);
}
/**
 * @brief Read data from the SPI bus and return a std::array of bytes.
 *
 * Filler bytes will be placed on the write line.
 *
 * @tparam BytesToRead - Number of bytes to read
 * @param p_spi - spi driver
 * @param p_filler - filler data placed on the bus in place of actual write
 * data.
 * @return boost::leaf::result<std::array<std::byte, BytesToRead>> - any errors
 * associated with this call
 */
template<size_t BytesToRead>
[[nodiscard]] boost::leaf::result<std::array<std::byte, BytesToRead>> read(
  spi& p_spi,
  std::byte p_filler = std::byte{ 0xFF }) noexcept
{
  std::array<std::byte, BytesToRead> buffer;
  BOOST_LEAF_CHECK(p_spi.transfer(std::span<std::byte>{}, buffer, p_filler));
  return buffer;
}

/**
 * @brief Write data to the SPI bus and ignore data sent from peripherals on the
 * bus then read data from the SPI and fill the write line with filler bytes.
 *
 * This utility function that fits the use case of many SPI devices where a
 * transaction is not full duplex. In many spi devices, full duplex means that
 * as data is being written to the SPI bus, the peripheral device is sending
 * data back on the read line. In most cases, the protocol is to write data to
 * the bus, and ignore the read line because the peripheral is not writing
 * anything meaningful to that line, then reading from SPI bus and writing
 * nothing meaningful to the write line as the peripheral is ignoring that line.
 *
 * @param p_spi - spi driver
 * @param p_data_out - bytes to write to the bus
 * @param p_data_in - buffer to receive bytes back from the SPI bus
 * @param p_filler - filler data placed on the bus when the read operation
 * begins.
 * @return boost::leaf::result<void>
 */
[[nodiscard]] inline boost::leaf::result<void> write_then_read(
  spi& p_spi,
  std::span<const std::byte> p_data_out,
  std::span<std::byte> p_data_in,
  std::byte p_filler = std::byte{ 0xFF }) noexcept
{
  BOOST_LEAF_CHECK(write(p_spi, p_data_out));
  BOOST_LEAF_CHECK(read(p_spi, p_data_in, p_filler));
  return {};
}

/**
 * @brief Write data to the SPI bus and ignore data sent from peripherals on the
 * bus then read data from the SPI, fill the write line with filler bytes and
 * return an array of bytes.
 *
 * @tparam BytesToRead - Number of bytes to read from the bus
 * @param p_spi - spi driver
 * @param p_data_out - bytes to write to the bus
 * @param p_filler - filler data placed on the bus when the read operation
 * begins.
 * @return boost::leaf::result<std::array<std::byte, BytesToRead>>
 */
template<size_t BytesToRead>
[[nodiscard]] boost::leaf::result<std::array<std::byte, BytesToRead>>
write_then_read(spi& p_spi,
                std::span<const std::byte> p_data_out,
                std::byte p_filler = std::byte{ 0xFF }) noexcept
{
  BOOST_LEAF_CHECK(write(p_spi, p_data_out));
  return read<BytesToRead>(p_spi, p_filler);
}
}  // namespace embed