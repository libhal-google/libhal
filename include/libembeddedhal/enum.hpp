#pragma once

#include <type_traits>

namespace embed {
/**
 * @addtogroup utility
 * @{
 */

/**
 * @brief concept for enumeration types
 *
 * @tparam T - enum type
 */
template<typename T>
concept enumeration = std::is_enum_v<T>;

/**
 * @brief Helper function to convert an enum to its integral value
 *
 * @param p_enum_value - the enumeration you want to convert into an integral
 * value
 * @return constexpr auto - return the integral value of the enum with the same
 * type as the enumeration.
 */
[[nodiscard]] constexpr auto value(enumeration auto p_enum_value) noexcept
{
  return static_cast<std::underlying_type_t<decltype(p_enum_value)>>(
    p_enum_value);
}
/** @} */
}  // namespace embed
