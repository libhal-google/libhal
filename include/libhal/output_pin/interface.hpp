#pragma once

#include "../error.hpp"
#include "../input_pin/pin_resistors.hpp"

namespace hal {
/**
 * @addtogroup output_pin Output Pin
 * Available output pin APIs
 * @{
 */
/**
 * @brief Digital output pin hardware abstraction.
 *
 * Use this to drive a pin HIGH or LOW in order to send a control signal or turn
 * off or on an LED.
 *
 */
class output_pin
{
public:
  /// Generic settings for output pins
  struct settings
  {
    /// Pull resistor for the pin. This generally only helpful when open
    /// drain is enabled.
    pin_resistor resistor = pin_resistor::pull_up;
    /// Starting level of the output pin. HIGH voltage defined as true and LOW
    /// voltage defined as false.
    bool open_drain = false;
    /// Set the starting level of the output pin on initialization. HIGH voltage
    /// defined as true and LOW voltage defined as false.
    bool starting_level = true;

    /**
     * @brief Default operators for <, <=, >, >= and ==
     *
     * @return auto - result of the comparison
     */
    [[nodiscard]] constexpr auto operator<=>(const settings&) const noexcept =
      default;
  };

  /**
   * @brief Configure the output pin to match the settings supplied
   *
   * @param p_settings - settings to apply to output pin
   * @return status
   * @throws std::errc::invalid_argument if the settings could not be achieved.
   */
  [[nodiscard]] status configure(const settings& p_settings) noexcept
  {
    return driver_configure(p_settings);
  }
  /**
   * @brief Set the state of the pin
   *
   * @param p_high - if true then the pin state is set to HIGH voltage. If
   * false, the pin state is set to LOW voltage.
   * @return status - any error that occurred during this
   * operation.
   */
  [[nodiscard]] status level(bool p_high) noexcept
  {
    return driver_level(p_high);
  }
  /**
   * @brief Read the state of the output pin. Implementations must read the pin
   * state from hardware and will not simply cache the results from running
   * level(bool).
   *
   * @return result<bool> - true indicates HIGH voltage and false
   * indicates LOW voltage
   */
  [[nodiscard]] result<bool> level() noexcept
  {
    return driver_level();
  }

private:
  virtual status driver_configure(const settings& p_settings) noexcept = 0;
  virtual status driver_level(bool p_high) noexcept = 0;
  virtual result<bool> driver_level() noexcept = 0;
};
/** @} */
}  // namespace hal
