#pragma once

#include "spi.hpp"

namespace embed::stub {
/**
 * @brief A stub implementation of embed::spi to be used as a placeholder for
 * drivers that require an optional spi interface. This implementation should
 * only be used when a driver has stated in its documentation that the spi
 * interface is optional and can be stubbed out.
 */
class spi : public embed::spi
{
private:
  boost::leaf::result<void> driver_configure(
    [[maybe_unused]] const settings& p_settings) noexcept
  {
    return {};
  }

  boost::leaf::result<void> driver_transfer(
    [[maybe_unused]] std::span<const std::byte> p_data_out,
    [[maybe_unused]] std::span<std::byte> p_data_in,
    [[maybe_unused]] std::byte p_filler) noexcept
  {
    return {};
  }
};
}  // namespace embed::stub
