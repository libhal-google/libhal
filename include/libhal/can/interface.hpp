#pragma once

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>

#include "../error.hpp"
#include "../units.hpp"

namespace hal {
/**
 * @addtogroup can
 * Available Controller Area Network (CAN bus) APIs
 * @{
 */
/**
 * @brief Controller Area Network (CAN bus) hardware abstraction interface.
 *
 */
class can
{
public:
  /// Generic settings for a can peripheral
  struct settings
  {
    /// Bus clock rate
    hertz clock_rate{};
  };

  /// Can message ID type trait
  using id_t = uint32_t;

  /// Structure of a CAN message
  struct message_t
  {
    /// ID of the message
    id_t id;
    /// The number of elements in the payload
    uint8_t length = 0;
    /// The message data
    std::array<hal::byte, 8> payload{ hal::byte{ 0 } };
    /// Whether or not the message is a remote request frame. If true, then
    /// length and payload are ignored.
    bool is_remote_request = false;
  };
  /**
   * @brief Configure can to match the settings supplied
   *
   * @param p_settings - settings to apply to can driver
   * @return status - success or failure
   * @throws std::errc::invalid_argument if the settings could not be achieved.
   */
  [[nodiscard]] status configure(const settings& p_settings) noexcept
  {
    return driver_configure(p_settings);
  }
  /**
   * @brief Send a can message over the can bus
   *
   * @param p_message - the message to be sent
   *
   * @return status - success or failure
   * operation.
   */
  [[nodiscard]] status send(const message_t& p_message) noexcept
  {
    return driver_send(p_message);
  }

  /**
   * @brief Setup driver to execute callback when a can message is received.
   *
   * All received can messages are dropped before this function is called.
   *
   * @param p_receive_handler - this handler will be called when the can device
   * receives a message. Set to nullptr to disable receive interrupts.
   * @return status - success or failure
   * operation.
   */
  [[nodiscard]] status attach_interrupt(
    std::function<void(const message_t& p_message)> p_receive_handler) noexcept
  {
    return driver_attach_interrupt(p_receive_handler);
  }

private:
  virtual status driver_configure(const settings& p_settings) noexcept = 0;
  virtual status driver_send(const message_t& p_message) noexcept = 0;
  virtual status driver_attach_interrupt(
    std::function<void(const message_t& p_message)>
      p_receive_handler) noexcept = 0;
};
/** @} */
}  // namespace hal
