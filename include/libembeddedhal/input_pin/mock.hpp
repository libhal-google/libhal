#pragma once

#include <queue>
#include <stdexcept>

#include "../testing.hpp"
#include "interface.hpp"

namespace embed::mock {
/**
 * @brief Mock input_pin implementation for use in unit tests and simulations.
 *
 */
struct input_pin : public embed::input_pin
{
  /**
   * @brief Reset spy information for configure()
   *
   */
  void reset() { spy_configure.reset(); }

  /// Spy handler for embedd:input_pin::configure()
  spy_handler<settings> spy_configure;

  /**
   * @brief Set input value of pin to given value
   *
   * @param p_level - vector of high or low values for input pin
   */
  void set(std::queue<bool>& p_levels) { m_level = p_levels; }

private:
  boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  }
  boost::leaf::result<bool> driver_level() noexcept
  {
    if (m_level.size() == 0) {
      return boost::leaf::new_error(
        std::out_of_range("input_pin level queue is empty!"));
    }
    bool m_current_value = m_level.front();
    m_level.pop();
    return m_current_value;
  }

  std::queue<bool> m_level{};
};
}  // namespace embed::mock
