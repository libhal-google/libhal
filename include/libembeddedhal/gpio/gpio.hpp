#pragma once

#include "../driver.hpp"

#include <functional>

namespace embed {
/**
 * @brief Set of possible pin mode resistor settings. See each enumeration to
 * get more details about when and how these should be used.
 *
 */
enum class pin_resistor
{
  /**
   * @brief No pull up. This will cause the pin to float. This may be desirable
   * if the pin has an external resistor attached or if the signal is sensitive
   * to external devices like resistors.
   *
   */
  none = 0,
  /**
   * @brief Pull the pin down to devices GND. This will ensure that the voltage
   * read by the pin when there is no signal on the pin is LOW (or false).
   *
   */
  pull_down,
  /**
   * @brief See pull down explanation, but in this case the pin is pulled up to
   * VCC, also called VDD on some systems.
   *
   */
  pull_up,
};

/**
 * @brief Generic settings for input pins
 *
 */
struct input_pin_settings
{
  /**
   * @brief pull resistor for an input pin
   *
   */
  pin_resistor resistor = pin_resistor::pull_up;
};

/**
 * @brief Generic settings for output pins
 *
 */
struct output_pin_settings
{
  /**
   * @brief Set the starting level of the output pin on initialization. HIGH
   * voltage defined as true and LOW voltage defined as false.
   *
   */
  bool starting_level = true;
  /**
   * @brief Starting level of the output pin. HIGH voltage defined as true and
   * LOW voltage defined as false.
   *
   */
  bool open_drain = false;
  /**
   * @brief Pull resistor for the pin. This generally only helpful when open
   * drain is enabled.
   *
   */
  pin_resistor resistor = pin_resistor::pull_up;
};

/**
 * @brief Generic settings for interrupt pins
 *
 */
struct interrupt_pin_settings
{
  /**
   * @brief pull resistor for an interrupt pin. Generally advised to NOT use
   * `pin_resistor::none` and if it is used and external pull resistor should be
   * placed on the pin to prevent random interrupt from firing.
   *
   */
  pin_resistor resistor = pin_resistor::pull_up;
};

/**
 * @brief Digital input pin hardware abstraction. Use this to read a pin and
 * determine if the voltage on it is HIGH or LOW.
 *
 */
class input_pin : public driver<input_pin_settings>
{
public:
  /**
   * @brief Read the state of the input pin
   *
   * @return true indicates a HIGH voltage
   * @return false indicates a LOW voltage
   */
  virtual bool level() const = 0;
};
/**
 * @brief Digital output pin hardware abstraction. Use this to drive a pin HIGH
 * or LOW in order to send a control signal or turn off or on an LED.
 *
 */
class output_pin : public driver<output_pin_settings>
{
public:
  /**
   * @brief Set the state of the pin
   *
   * @param p_high if true then the pin state is set to HIGH voltage. If false,
   * the pin state is set to LOW voltage.
   */
  virtual void level(bool p_high) = 0;
  /**
   * @brief Read the state of the input pin. Implementations must read the pin
   * state from hardware and will not simply cache the results from running
   * level(bool).
   *
   * @return true indicates a HIGH voltage
   * @return false indicates a LOW voltage
   */
  virtual bool level() const = 0;
};
/**
 * @brief Digital interrupt pin hardware abstraction. Use this to trigger an
 * interrupt service routine (ISR) when a pin detects a falling edge (when the
 * pin's voltage transitions from HIGH to LOW), a rising edge (when the pin's
 * voltage transitions from LOW to HIGH), or any transition of state on the pin.
 *
 */
class interrupt_pin : public driver<interrupt_pin_settings>
{
public:
  /**
   * @brief The condition in which an interrupt is triggered.
   *
   */
  enum class trigger_edge
  {
    /**
     * @brief Trigger and interrupt when a pin transitions from HIGH voltage to
     * LOW voltage.
     *
     */
    falling = 0,
    /**
     * @brief Trigger and interrupt when a pin transitions from LOW voltage to
     * HIGH voltage.
     *
     */
    rising = 1,
    /**
     * @brief Trigger and interrupt when a pin transitions it state
     *
     */
    both = 2,
  };
  /**
   * @brief Return the voltage level of the pin
   *
   * @return true HIGH voltage
   * @return false LOW voltage
   */
  virtual bool level() const = 0;
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
   */
  virtual void attach_interrupt(std::function<void(void)> p_callback,
                                trigger_edge p_trigger) = 0;
  /**
   * @brief Disable interrupts for this pin
   *
   */
  virtual void detach_interrupt() = 0;
};
} // namespace embed
