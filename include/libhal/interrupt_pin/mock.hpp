#pragma once

#include "../testing.hpp"
#include "interface.hpp"

namespace hal::mock {
/**
 * @addtogroup interrupt_pin Interrupt Pin
 * @{
 */
/**
 * @brief mock interrupt_pin implementation for use in unit tests and
 * simulations.
 *
 */
struct interrupt_pin : public hal::interrupt_pin
{
  /**
   * @brief Reset spy information for configure(), on_trigger(), and
   * disable()
   *
   */
  void reset()
  {
    spy_configure.reset();
    spy_on_trigger.reset();
  }

  /// Spy handler for hal::interrupt_pin::configure()
  spy_handler<settings> spy_configure;
  /// Spy handler for hal::interrupt_pin::on_trigger()
  spy_handler<std::function<handler>> spy_on_trigger;

private:
  status driver_configure(const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  }
  void driver_on_trigger(std::function<handler> p_callback) noexcept override
  {
    [[maybe_unused]] auto result = spy_on_trigger.record(p_callback);
  }
};
/** @} */
}  // namespace hal::mock
