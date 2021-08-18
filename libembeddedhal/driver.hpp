#pragma once

namespace embed {
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

  [[nodiscard]] settings_t& settings() const { return m_settings; }
  [[nodiscard]] const settings_t& initialized_settings() const
  {
    return m_settings;
  }
  [[nodiscard]] bool is_initialized() const { return m_initialized; }
  void reset() { m_initialized = false; }

  // TODO: add virtual destructor

protected:
  virtual bool driver_initialize() = 0;
  settings_t m_settings{};
  settings_t m_initialized_settings{};
  bool m_initialized = false;
};
} // namespace embed
