#pragma once

#include <array>
#include <cstdint>

#include "error.hpp"
#include "functional.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief Controller Area Network (CAN bus) hardware abstraction interface.
 *
 */
class can
{
public:
  /// Can message ID type trait
  using id_t = uint32_t;

  /// Generic settings for a can peripheral
  ///
  /// CAN Bit Quanta Timing Diagram of:
  ///
  ///                              | <--- sjw ---> |
  ///        ____    ______    __________    __________
  ///     _/ SYNC \/  PROP  \/   PHASE1   \/   PHASE2   \_
  ///      \______/\________/\____________/\____________/
  ///                                      ^ Sample point
  ///
  struct settings
  {
    /// Bus clock rate in hertz
    hertz baud_rate = 100.0_kHz;

    /// Sync Segment (always 1qt)
    ///
    /// Initial sync transition, the start of a CAN bit
    static constexpr std::uint8_t sync_segment = 1;

    /// Propagation Delay (1qt ... 8qt)
    ///
    /// Propagation time It is used to compensate for signal delays across the
    /// network.
    std::uint8_t propagation_delay = 3;

    /// Length of Phase Segment 1 (1qt ... 8qt)
    ///
    /// Determines the bit rate, phase segment 1 acts as a buffer that can be
    /// lengthened to resynchronize with the bit stream via the
    /// synchronization_jump_width. Includes propagation delay
    std::uint8_t phase_segment1 = 3;

    /// Length of Phase Segment 2 (1qt ... 8qt)
    ///
    /// Determines the bit rate and is like phase segment 1 and occurs after the
    /// sampling point. Phase segment 2 can be shortened to resynchronize with
    /// the bit stream via the synchronization_jump_width.
    std::uint8_t phase_segment2 = 3;

    /// Synchronization jump width (1qt ... 4qt)
    ///
    /// This is the maximum time by which the bit sampling period may be
    /// lengthened or shortened during each cycle to adjust for oscillator
    /// mismatch between nodes.
    ///
    /// This value must be smaller than phase_segment1 and phase_segment2
    std::uint8_t synchronization_jump_width = 1;
  };

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
  [[nodiscard]] status configure(const settings& p_settings)
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
  [[nodiscard]] status send(const message_t& p_message)
  {
    return driver_send(p_message);
  }

  /**
   * @brief Set the message reception handler
   *
   * Before this function is called all received messages are dropped.
   *
   * @param p_handler - this handler will be called when a message has been
   * received.
   * @return status - success or failure
   */
  [[nodiscard]] status on_receive(hal::callback<handler> p_handler)
  {
    return driver_on_receive(p_handler);
  }

  virtual ~can() = default;

private:
  virtual status driver_configure(const settings& p_settings) = 0;
  virtual status driver_send(const message_t& p_message) = 0;
  virtual status driver_on_receive(hal::callback<handler> p_handler) = 0;
};
}  // namespace hal
