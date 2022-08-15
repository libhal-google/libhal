#pragma once

#include <array>
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
    /// Bus clock rate in hertz
    std::uint32_t clock_rate{};

    /**
     * @brief Default operators for <, <=, >, >= and ==
     *
     * @return auto - result of the comparison
     */
    [[nodiscard]] constexpr auto operator<=>(const settings&) const noexcept =
      default;
  };

  /// Can message ID type trait
  using id_t = uint32_t;

  /// Structure of a CAN message
  struct message_t
  {
    /// ID of the message
    id_t id;
    /// The message data
    std::array<hal::byte, 8> payload{};
    /// The number of elements in the payload
    uint8_t length = 0;
    /// Whether or not the message is a remote request frame. If true, then
    /// length and payload are ignored.
    bool is_remote_request = false;
  };

  // Receive handler for can messages
  using handler = void(const message_t& p_message);

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
   * @brief Send a message
   *
   * @param p_message - the message to be sent
   *
   * @return status - success or failure
   */
  [[nodiscard]] status send(const message_t& p_message) noexcept
  {
    return driver_send(p_message);
  }

  /**
   * @brief Set the message reception handler
   *
   * Before this function is called all received messages are dropped.
   *
   * @param p_handler - this handler will be called when a message has been
   * received. Set to "nullptr" to disable receive interrupts.
   * @return status - success or failure
   */
  [[nodiscard]] status on_receive(std::function<handler> p_handler) noexcept
  {
    return driver_on_receive(p_handler);
  }

private:
  virtual status driver_configure(const settings& p_settings) noexcept = 0;
  virtual status driver_send(const message_t& p_message) noexcept = 0;
  virtual status driver_on_receive(std::function<handler> p_handler) noexcept;
};
/** @} */
}  // namespace hal
