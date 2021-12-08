#pragma once

#include "../driver.hpp"

#include <array>
#include <chrono>
#include <cinttypes>
#include <functional>

namespace embed {
/**
 * @brief Generic settings for a can peripheral
 *
 */
struct can_settings
{
  /// Bus clock rate
  uint32_t clock_rate_hz = 100'000;
};

/**
 * @brief Controller Area Network (CAN bus) hardware abstraction interface.
 *
 */
class can : public driver<can_settings>
{
public:
  /// Can message ID type trait
  using id_t = uint32_t;

  /**
   * @brief Attributes of a can message
   *
   */
  struct message_t
  {
    /// ID of the message
    id_t id;
    /// The number of elements in the payload
    uint8_t length = 0;
    /// The message data
    std::array<uint8_t, 8> payload{ 0 };
    /// Whether or not the message is a remote request frame. If true, then
    /// length and payload are ignored.
    bool is_remote_request = false;
  };

  /**
   * @brief Send a can message over the can bus
   *
   * @param p_message the message to be sent
   */
  virtual void send(const message_t& p_message) = 0;

  /**
   * @brief Determine if the can bus has any queued up can messages
   *
   * @return true a can message is available to be read
   * @return false no messages available to be read
   */
  virtual bool has_data() = 0;

  /**
   * @brief Receieve a message from the can bus if one is available.
   *
   * Behavior is undefined if no data is available.
   *
   * @return message_t can message from the bus
   */
  virtual message_t receive() = 0;

  /**
   * @brief Setup driver to execute callback when a can message is received
   *
   * @param p_receive_handler - can message reception callback. Set to nullptr
   * to disable receive interrupts.
   */
  virtual void attach_interrupt(
    std::function<void(can& p_can)> p_receive_handler) = 0;
};
}  // namespace embed
