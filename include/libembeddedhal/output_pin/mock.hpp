#pragma once

#include "../testing.hpp"
#include "interface.hpp"

namespace embed::mock {
/**
 * @addtogroup output_pin Output Pin
 * @{
 */
/**
 * @brief mock output pin for use in unit tests and simulations
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
  /// Spy handler for embed::output_pin::level()
  spy_handler<bool> spy_level;

private:
  boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  }
  boost::leaf::result<void> driver_level(bool p_high) noexcept override
  {
    m_current_level = p_high;
    return spy_level.record(p_high);
  }
  boost::leaf::result<bool> driver_level() noexcept override
  {
    return m_current_level;
  }
  bool m_current_level = false;
};
/** @} */
}  // namespace embed::mock
