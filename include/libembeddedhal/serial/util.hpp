#pragma once

#include <optional>
#include <span>
#include <string_view>

#include "../error.hpp"
#include "../time.hpp"
#include "interface.hpp"

namespace embed {

[[nodiscard]] inline boost::leaf::result<void> wait_for(
  serial& p_serial,
  size_t p_length) noexcept
{
  size_t bytes_available = BOOST_LEAF_CHECK(p_serial.bytes_available());
  while (bytes_available < p_length) {
    bytes_available = BOOST_LEAF_CHECK(p_serial.bytes_available());
  }
  return {};
}

[[nodiscard]] inline boost::leaf::result<void> write(
  serial& p_serial,
  std::span<const std::byte> p_data_out) noexcept
{
  return p_serial.write(p_data_out);
}

[[nodiscard]] inline boost::leaf::result<std::span<const std::byte>> read(
  serial& p_serial,
  std::span<std::byte> p_data_in)
{
  BOOST_LEAF_CHECK(wait_for(p_serial, p_data_in.size()));
  return p_serial.read(p_data_in);
}

template<size_t BytesToRead>
[[nodiscard]] boost::leaf::result<std::array<std::byte, BytesToRead>> read(
  serial& p_serial) noexcept
{
  std::array<std::byte, BytesToRead> buffer;
  BOOST_LEAF_CHECK(wait_for(p_serial, BytesToRead));
  BOOST_LEAF_CHECK(p_serial.read(buffer));
  return buffer;
}

[[nodiscard]] inline boost::leaf::result<void> write_then_read(
  serial& p_serial,
  std::span<const std::byte> p_data_out,
  std::span<std::byte> p_data_in) noexcept
{
  BOOST_LEAF_CHECK(write(p_serial, p_data_out));
  BOOST_LEAF_CHECK(read(p_serial, p_data_in));
  return {};
}

template<size_t BytesToRead>
[[nodiscard]] boost::leaf::result<std::array<std::byte, BytesToRead>>
write_then_read(serial& p_serial,
                std::span<const std::byte> p_data_out) noexcept
{
  std::array<std::byte, BytesToRead> buffer;
  BOOST_LEAF_CHECK(write_then_read(p_serial, p_data_out, buffer));
  return buffer;
}
}  // namespace embed