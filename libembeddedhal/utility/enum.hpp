#pragma once

namespace embed {
template<typename enum_type>
constexpr auto value(enum_type enum_value)
{
  return static_cast<std::underlying_type_t<enum_type>>(enum_value);
}
} // namespace embed
