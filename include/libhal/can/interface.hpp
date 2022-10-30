#pragma once

#include <array>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iosfwd>
#include <optional>

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

    /**
     * @brief Default operators for <, <=, >, >= and ==
     *
     * @return auto - result of the comparison
     */
    [[nodiscard]] constexpr auto operator<=>(const settings&) const noexcept =
      default;

    /**
     * @brief Operators ==
     *
     * @return auto - result of the comparison
     */
    [[nodiscard]] constexpr auto operator==(
      const settings& p_settings) const noexcept
    {
      return equals(baud_rate, p_settings.baud_rate);
    }

    [[nodiscard]] constexpr std::uint16_t bit_width() const noexcept
    {
      // The sum of 4x 8-bit numbers can never exceed uint16_t and thus this
      // operation is always safe.
      return static_cast<std::uint16_t>(sync_segment + propagation_delay +
                                        phase_segment1 + phase_segment2);
    }

    /**
     * @brief Validate configuration settings against an operating frequency
     *
     * The settings and frequency must follow the following rules:
     *
     * 1. propagation_delay, phase_segment1, phase_segment2 and
     *    synchronization_jump_width must be nonzero.
     * 2. synchronization_jump_width must be the lesser between phase_segment1
     *    and phase_segment2.
     * 3. The total bit width must be equal to or greater than 8 Tq/bit; the sum
     *    of sync_segment, propagation_delay, phase_segment1 and phase_segment2.
     * 4. The CAN device's operating frequency must be at least 8 times the baud
     *    rate to give the minimum.
     * 5. The ratio between the CAN device's operating frequency and the bit
     *    width must be close enough to an integer to produce a usable baud
     *    rate prescaler.
     *
     * @param p_operating_frequency - CAN device operating frequency
     * @return std::optional<std::uint32_t> - baud rate prescaler
     */
    [[nodiscard]] constexpr std::optional<std::uint32_t> is_valid(
      hertz p_operating_frequency) const noexcept
    {
      // 1. propagation_delay, phase_segment1, phase_segment2 and
      //    synchronization_jump_width must be nonzero.
      if (propagation_delay == 0 || phase_segment1 == 0 ||
          phase_segment2 == 0 || synchronization_jump_width == 0) {
        return std::nullopt;
      }

      // 2. synchronization_jump_width must be the lesser between phase_segment1
      //    and phase_segment2.
      if (synchronization_jump_width > 4 ||
          synchronization_jump_width > phase_segment1 ||
          synchronization_jump_width > phase_segment2) {
        return std::nullopt;
      }

      const std::uint16_t bit_width_v = bit_width();

      // 3. The total bit width must be equal to or greater than 8 Tq/bit; the
      //    sum of sync_segment, propagation_delay, phase_segment1 and
      //    phase_segment2.
      if (bit_width_v < 8) {
        return std::nullopt;
      }

      // 4. The CAN device's operating frequency must be at least 8 times the
      //    bit rate to give the minimum.
      // 5. The ratio between the CAN device's operating frequency and the bit
      //    width must be close enough to an integer to produce a usable BRP.
      const float bit_width_float = bit_width_v;
      const float scaled_baud = baud_rate * bit_width_float;
      const float baud_rate_prescaler = p_operating_frequency / scaled_baud;
      const auto final_prescaler = std::lround(baud_rate_prescaler);

      if (final_prescaler == 0) {
        return std::nullopt;
      }

      return final_prescaler;
    }
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

    /**
     * @brief Default operators for <, <=, >, >= and ==
     *
     * @return auto - result of the comparison
     */
    [[nodiscard]] constexpr auto operator<=>(const message_t&) const noexcept =
      default;

    /**
     * @brief print this type using ostreams
     *
     * Meant for unit testing, testing and simulation purposes
     * C++ streams, in general, should not be used for any embedded project that
     * will ever have to be used on an MCU due to its memory cost.
     *
     * @param p_ostream - the ostream
     * @param p_message - object to convert to a string
     * @return std::ostream& - reference to the ostream
     */
    friend std::ostream& operator<<(std::ostream& p_ostream,
                                    const message_t& p_message)
    {
      p_ostream << "{ id: " << std::hex << "0x" << p_message.id;
      p_ostream << ", length: " << std::dec << unsigned{ p_message.length };
      p_ostream << ", is_remote_request: " << p_message.is_remote_request;
      p_ostream << ", payload = [";
      for (const auto& element : p_message.payload) {
        p_ostream << std::hex << "0x" << unsigned{ element } << ", ";
      }
      p_ostream << "] }";
      return p_ostream;
    }
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
  virtual status driver_on_receive(
    std::function<handler> p_handler) noexcept = 0;
};
/** @} */
}  // namespace hal
