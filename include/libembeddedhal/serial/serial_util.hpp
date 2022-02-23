#pragma once

#include <optional>
#include <span>
#include <string_view>

#include "../error.hpp"
#include "../time.hpp"
#include "serial.hpp"

namespace embed {

[[nodiscard]] inline boost::leaf::result<void> wait(
  serial& p_serial,
  size_t p_length,
  std::optional<std::chrono::nanoseconds> p_timeout = std::nullopt) noexcept
{
  if (p_timeout) {
    auto current_time = this_thread::uptime();
    auto timeout_time = current_time + p_timeout.value();
    size_t bytes_available = BOOST_LEAF_CHECK(p_serial.bytes_available());

    while (bytes_available < p_length) {
      current_time = this_thread::uptime();
      if (current_time > timeout_time) {
        return boost::leaf::new_error(error::timeout{});
      }
      bytes_available = BOOST_LEAF_CHECK(p_serial.bytes_available());
    }
  } else {
    size_t bytes_available = BOOST_LEAF_CHECK(p_serial.bytes_available());
    while (bytes_available < p_length) {
      bytes_available = BOOST_LEAF_CHECK(p_serial.bytes_available());
    }
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
  std::span<std::byte> p_data_in,
  std::optional<std::chrono::nanoseconds> p_timeout = std::nullopt)
{
  BOOST_LEAF_CHECK(wait(p_serial, p_data_in.size(), p_timeout));
  return p_serial.read(p_data_in);
}

template<size_t BytesToRead>
[[nodiscard]] boost::leaf::result<std::array<std::byte, BytesToRead>> read(
  serial& p_serial,
  std::optional<std::chrono::nanoseconds> p_timeout = std::nullopt) noexcept
{
  std::array<std::byte, BytesToRead> buffer;
  BOOST_LEAF_CHECK(wait(p_serial, BytesToRead, p_timeout));
  BOOST_LEAF_CHECK(p_serial.read(buffer));
  return buffer;
}

[[nodiscard]] inline boost::leaf::result<void> write_then_read(
  serial& p_serial,
  std::span<const std::byte> p_data_out,
  std::span<std::byte> p_data_in,
  std::optional<std::chrono::nanoseconds> p_timeout = std::nullopt) noexcept
{
  BOOST_LEAF_CHECK(write(p_serial, p_data_out));
  BOOST_LEAF_CHECK(read(p_serial, p_data_in, p_timeout));
  return {};
}

template<size_t BytesToRead>
[[nodiscard]] boost::leaf::result<std::array<std::byte, BytesToRead>>
write_then_read(
  serial& p_serial,
  std::span<const std::byte> p_data_out,
  std::optional<std::chrono::nanoseconds> p_timeout = std::nullopt) noexcept
{
  std::array<std::byte, BytesToRead> buffer;
  BOOST_LEAF_CHECK(write_then_read(p_serial, p_data_out, buffer, p_timeout));
  return buffer;
}
}  // namespace embed