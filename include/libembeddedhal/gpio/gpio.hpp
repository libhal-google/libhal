#pragma once

#include "../driver.hpp"

#include <functional>

namespace embed {
/// Set of possible pin mode resistor settings. See each enumeration to get more
/// details about when and how these should be used.
enum class pin_resistor
{
  /// No pull up. This will cause the pin to float. This may be desirable if the
  /// pin has an external resistor attached or if the signal is sensitive to
  /// external devices like resistors.
  none = 0,
  /// Pull the pin down to devices GND. This will ensure that the voltage read
  /// by the pin when there is no signal on the pin is LOW (or false).
  pull_down,
  /// See pull down explanation, but in this case the pin is pulled up to VCC,
  /// also called VDD on some systems.
  pull_up,
};

/// Settings for input pins
struct input_pin_settings
{
  pin_resistor resistor = pin_resistor::pull_up;
};

struct output_pin_settings
{
  bool starting_level = true;
  bool open_drain = false;
  pin_resistor resistor = pin_resistor::pull_up;
};

struct interrupt_pin_settings
{
  pin_resistor resistor = pin_resistor::pull_up;
};

class input_pin : public driver<input_pin_settings>
{
public:
  virtual bool level() const = 0;
};

class output_pin : public driver<output_pin_settings>
{
public:
  virtual void level(bool p_high) = 0;
  virtual bool level() const = 0;
};

class interrupt_pin : public driver<interrupt_pin_settings>
{
public:
  enum class trigger_edge
  {
    falling = 0,
    rising = 1,
    both = 2,
  };

  virtual bool level() const = 0;
  virtual void attach_interrupt(std::function<void(void)> p_callback,
                                trigger_edge p_trigger) = 0;
  virtual void detach_interrupt() = 0;
};
} // namespace embed
