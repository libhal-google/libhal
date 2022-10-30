#pragma once

#include <queue>
#include <stdexcept>

#include "../testing.hpp"
#include "interface.hpp"

namespace hal::mock {
/**
 * @addtogroup input_pin Input Pin
 * @{
 */
/**
 * @brief mock input_pin implementation for use in unit tests and simulations.
 *
 */
struct input_pin : public hal::input_pin
{
  /**
   * @brief Reset spy information for configure()
   *
   */
  void reset()
  {
    spy_configure.reset();
  }

  /// Spy handler for embed:input_pin::configure()
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
  status driver_configure(const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  }
  result<bool> driver_level() noexcept override
  {
    // This comparison performs bounds checking because front() and pop() do
    // not bounds check and results in undefined behavior if the queue is empty.
    if (m_levels.size() == 0) {
      return hal::new_error(
        std::out_of_range("input_pin level queue is empty!"));
    }
    bool m_current_value = m_levels.front();
    m_levels.pop();
    return m_current_value;
  }

  std::queue<bool> m_levels{};
};
/** @} */
}  // namespace hal::mock
