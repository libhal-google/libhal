#pragma once

#include "../testing.hpp"
#include "interface.hpp"

namespace hal::mock {
/**
 * @addtogroup output_pin Output Pin
 * @{
 */
/**
 * @brief mock output pin for use in unit tests and simulations
 *
 */
struct output_pin : public hal::output_pin
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

  /// Spy handler for hal::output_pin::configure()
  spy_handler<settings> spy_configure;
  /// Spy handler for hal::output_pin::level()
  spy_handler<bool> spy_level;

private:
  status driver_configure(const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  }
  status driver_level(bool p_high) noexcept override
  {
    m_current_level = p_high;
    return spy_level.record(p_high);
  }
  result<bool> driver_level() noexcept override
  {
    return m_current_level;
  }
  bool m_current_level = false;
};
/** @} */
}  // namespace hal::mock
