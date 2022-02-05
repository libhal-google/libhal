#pragma once

#include <functional>

#include "../error.hpp"
#include "pin_resistors.hpp"

namespace embed {
/**
 * @brief Digital interrupt pin hardware abstraction.
 *
 * Use this to trigger an interrupt service routine (ISR) when a pin detects a
 * falling edge (when the pin's voltage transitions from HIGH to LOW), a rising
 * edge (when the pin's voltage transitions from LOW to HIGH), or any transition
 * of state on the pin.
 *
 */
class interrupt_pin
{
public:
  /// Generic settings for interrupt pins
  struct settings
  {
    /// pull resistor for an interrupt pin. Generally advised to NOT use
    /// `pin_resistor::none` and if it is used and external pull resistor should
    /// be placed on the pin to prevent random interrupt from firing.
    pin_resistor resistor = pin_resistor::pull_up;
  };

  /// The condition in which an interrupt is triggered.
  enum class trigger_edge
  {
    /// Trigger and interrupt when a pin transitions from HIGH voltage to
    /// LOW voltage.
    falling = 0,
    /// Trigger and interrupt when a pin transitions from LOW voltage to
    /// HIGH voltage.
    rising = 1,
    /// Trigger and interrupt when a pin transitions it state
    both = 2,
  };
  /**
   * @brief Configure the interrupt pin to match the settings supplied
   *
   * @param p_settings - settings to apply to interrupt pin
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation. Will return embed::error::invalid_settings if the settings could
   * not be achieved.
   */
  [[nodiscard]] boost::leaf::result<void> configure(const settings& p_settings)
  {
    return driver_configure(p_settings);
  }
  /**
   * @brief Return the voltage level of the pin
   *
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  [[nodiscard]] boost::leaf::result<bool> level() { return driver_level(); }
  /**
   * @brief Enable interrupts for this pin. Pass in the callback to be executed
   * when the trigger condition is met. This function can be called multiple
   * times if the callback or trigger conditions need to be changed.
   * detach_interrupts() does not need to be called before re-running this
   * function.
   *
   * @param p_callback function to execute when the trigger condition is met
   * @param p_trigger the trigger condition that will signal the system to run
   * the callback.
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  [[nodiscard]] boost::leaf::result<void> attach_interrupt(
    std::function<void(void)> p_callback,
    trigger_edge p_trigger)
  {
    return driver_attach_interrupt(p_callback, p_trigger);
  }
  /**
   * @brief Disable interrupts for this pin
   *
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  [[nodiscard]] boost::leaf::result<void> detach_interrupt()
  {
    return driver_detach_interrupt();
  }

private:
  virtual boost::leaf::result<void> driver_configure(
    const settings& p_settings) = 0;
  virtual boost::leaf::result<bool> driver_level() = 0;
  virtual boost::leaf::result<void> driver_attach_interrupt(
    std::function<void(void)> p_callback,
    trigger_edge p_trigger) = 0;
  virtual boost::leaf::result<void> driver_detach_interrupt() = 0;
};
}  // namespace embed
