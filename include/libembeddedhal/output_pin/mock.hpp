#include "../testing.hpp"
#include "interface.hpp"

namespace embedd::mock {
/**
 * @brief Mock output pin for use in unit tests and simulations with spy
 *  functions for configure configure() and level()
 *
 */
struct output_pin : public embed::output_pin
{
  /**
   * @brief Reset spy information for configure() and level()
   *
   */
  void reset()
  {
    spy_configure.reset();
    spy_level.reset();
  }

  /// Spy handler for embed::output_pin::configure()
  spy_handler<settings> spy_configure;
  /// Spy handler for emned::output_pin::level()
  spy_handler<bool> spy_level;

private:
  boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  };
  boost::leaf::result<void> driver_level(bool p_high) noexcept override
  {
    return spy_level.record(p_high);
  };
};
}  // namespace embedd::mock