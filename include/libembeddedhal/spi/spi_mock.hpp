#include "../testing.hpp"
#include "spi.hpp"

namespace embed::mock {
/**
 * @brief Mock spi implementation for use in unit tests and simulations with spy
 * functions for configure() and transfer().
 *
 */
struct spi : public embed::spi
{
  /**
   * @brief Reset spy information for both configure() and transfer()
   *
   */
  void reset()
  {
    spy_configure.reset();
    spy_transfer.reset();
  }

  /// Spy handler for embed::spi::configure()
  spy_handler<settings> spy_configure;
  /// Spy handler for embed::spi::transfer()
  spy_handler<std::span<const std::byte>, std::span<std::byte>, std::byte>
    spy_transfer;

private:
  boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  };
  boost::leaf::result<void> driver_transfer(
    std::span<const std::byte> p_data_out,
    std::span<std::byte> p_data_in,
    std::byte p_filler) noexcept override
  {
    return spy_transfer.record(p_data_out, p_data_in, p_filler);
  };
};
}  // namespace embed::mock
