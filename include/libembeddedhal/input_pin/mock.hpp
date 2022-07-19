#pragma once

#include <queue>
#include <stdexcept>

#include "../testing.hpp"
#include "interface.hpp"

namespace embed::mock {
/**
 * @addtogroup input_pin Input Pin
 * @{
 */
/**
 * @brief mock input_pin implementation for use in unit tests and simulations.
 *
 */
struct input_pin : public embed::input_pin_interface
{
  /**
   * @brief Reset spy information for configure()
   *
   */
  void reset()
  {
    spy_configure.reset();
  }

  /// Spy handler for embedd:input_pin::configure()
  spy_handler<settings> spy_configure;

  /**
   * @brief Queues the active levels to be returned for levels()
   *
   * @param p_levels - queue of actives levels
   */
  void set(std::queue<bool>& p_levels)
  {
    m_levels = p_levels;
  }

private:
  boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  }
  boost::leaf::result<bool> driver_level() noexcept
  {
    // This comparison performs bounds checking because front() and pop() do
    // not bounds check and results in undefined behavior if the queue is empty.
    if (m_levels.size() == 0) {
      return boost::leaf::new_error(
        std::out_of_range("input_pin level queue is empty!"));
    }
    bool m_current_value = m_levels.front();
    m_levels.pop();
    return m_current_value;
  }

  std::queue<bool> m_levels{};
};
/** @} */
}  // namespace embed::mock
