#pragma once

#include "interface.hpp"

namespace embed::stub {
/**
 * A stub implementation of embed::serial to be used as a placeholder for
 * drivers that require an optional serial port. This implementation should only
 * be used when a driver has stated in its documentation that the serial port is
 * optional and can be stubbed out.
 *
 */
class serial : public embed::serial
{
private:
  boost::leaf::result<void> driver_configure(
    [[maybe_unused]] const settings& p_settings) noexcept
  {
    return {};
  }
  boost::leaf::result<void> driver_write(
    [[maybe_unused]] std::span<const std::byte> p_data) noexcept
  {
    return {};
  }

  boost::leaf::result<size_t> driver_bytes_available() noexcept { return 0; }

  boost::leaf::result<std::span<const std::byte>> driver_read(
    std::span<std::byte> p_data) noexcept
  {
    return p_data.subspan(0, 0);
  }
  boost::leaf::result<void> driver_flush() noexcept { return {}; }
};
}  // namespace embed::stub
