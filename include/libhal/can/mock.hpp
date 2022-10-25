#pragma once

#include "../testing.hpp"
#include "interface.hpp"

namespace hal::mock {
/**
 * @addtogroup can
 * @{
 */

/**
 * @brief Mock can implementation for use in unit tests and simulations
 *
 */
struct can : public hal::can
{
  /**
   * @brief Reset spy information for functions
   *
   */
  void reset()
  {
    spy_configure.reset();
    spy_send.reset();
    spy_on_receive.reset();
  }

  /// Spy handler for hal::can::configure()
  spy_handler<settings> spy_configure;
  /// Spy handler for hal::can::send()
  spy_handler<message_t> spy_send;
  /// Spy handler for hal::can::on_receive()
  spy_handler<std::function<handler>> spy_on_receive;

private:
  status driver_configure(const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  };
  status driver_send(const message_t& p_message) noexcept override
  {
    return spy_send.record(p_message);
  };
  status driver_on_receive(std::function<handler> p_handler) noexcept override
  {
    return spy_on_receive.record(p_handler);
  };
};
/** @} */
}  // namespace hal::mock
