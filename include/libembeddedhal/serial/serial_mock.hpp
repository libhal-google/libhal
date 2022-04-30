#include "../testing.hpp"
#include "serial.hpp"

namespace embed::mock {
/**
 * @brief Mock serial implementation for use in unit tests and simulations with spy
 * functions for configure(), write(), and read().
 *
 */
struct serial : public embed::serial
{
  /**
   * @brief Reset spy information for both configure(), write(), and spy_read().
   *
   */
  void reset()
  {
    spy_configure.reset();
    spy_write.reset();
    spy_read.reset();
  }

  /// Spy handler for embed::serial::configure()
  spy_handler<settings> spy_configure;
  /// Spy handler for embed::serial::write()
  spy_handler<std::span<const std::byte>> spy_write;
  /// Spy handler for embed::serial::read()
  spy_handler<std::span<std::byte>> spy_read;

private:

  boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  };
  boost::leaf::result<void> driver_write(
    std::span<const std::byte> p_data) noexcept override
  {
    return spy_write.record(p_data);
  };
  boost::leaf::result<std::span<const std::byte>> driver_read(
    std::span<std::byte> p_data) noexcept override
  {
    return spy_read.record(p_data);
  };
};
}  // namespace embed::mock