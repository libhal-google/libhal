#pragma once

#include <type_traits>
// This is included for convience purposes
#include "enum.hpp"

namespace embed {
/// Used for defining static_asserts that should always fail, but only if the
/// static_assert line is hit via `if constexpr` control block.
/// Prefer to NOT use this directly but to use `invalid_option` instead
template<auto... options>
struct invalid_option_t : std::false_type
{};

/// Helper definition to simplify the usage of invalid_option_t.
/// @tparam options
template<auto... options>
inline constexpr bool invalid_option = invalid_option_t<options...>::value;

/// An empty settings structure used to indicate that a driver or interface does
/// not have generic settings.
struct no_settings
{};

/// The basis class for all peripheral, device and system drivers in SJSU-Dev2.
template<class settings_t = no_settings>
class driver
{
public:
  [[nodiscard]] bool initialize()
  {
    bool success = false;

    if (!m_initialized) {
      success = driver_initialize();
      if (success) {
        m_initialized_settings = m_settings;
        m_initialized = true;
        success = true;
      }
    }

    return success;
  }

  [[nodiscard]] settings_t& settings() { return m_settings; }
  [[nodiscard]] const settings_t& initialized_settings() const
  {
    return m_settings;
  }
  [[nodiscard]] bool is_initialized() const { return m_initialized; }
  void reset() { m_initialized = false; }

protected:
  virtual bool driver_initialize() = 0;
  settings_t m_settings{};
  settings_t m_initialized_settings{};
  bool m_initialized = false;
};
} // namespace embed
