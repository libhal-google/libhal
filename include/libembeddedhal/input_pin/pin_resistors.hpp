#pragma once

namespace embed {
/**
 * @addtogroup input_pin Input Pin
 * @{
 */
/**
 * @brief Set of possible pin mode resistor settings.
 *
 * See each enumeration to get more details about when and how these should be
 * used.
 *
 */
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
/** @} */
}  // namespace embed
