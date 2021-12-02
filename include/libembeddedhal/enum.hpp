#pragma once

namespace embed {
/**
 * @brief Helper function to convert an enum to its integral value.
 *
 * @tparam enum_type the underlying type of the enumeration. Do not supply this
 * value, it is inferred by the enum_value passed to the function.
 * @param enum_value the enumeration you want to convert into an integral value
 * @return constexpr auto return the integral value of the enum.
 */
template<typename enum_type>
constexpr auto value(enum_type enum_value)
{
  return static_cast<std::underlying_type_t<enum_type>>(enum_value);
}
} // namespace embed
