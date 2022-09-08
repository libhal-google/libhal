#pragma once

#include <queue>

#include "interface.hpp"

namespace hal::mock {
/**
 * @addtogroup steady_clock
 * @{
 */
/**
 * @brief mock steady_clock implementation for use in unit tests and
 * simulations.
 *
 */
struct steady_clock : public hal::steady_clock
{
  /**
   * @brief Set the frequency to be returned from frequency()
   *
   * @param p_frequency - Frequency to return
   */
  void set_frequency(hertz p_frequency)
  {
    m_frequency = p_frequency;
  }

  /**
   * @brief Queues the uptimes to be returned from uptimes()
   *
   * @param p_uptime_values - Queue of uptimes
   */
  void set_uptimes(std::queue<std::uint64_t>& p_uptime_values)
  {
    m_uptime_values = p_uptime_values;
  }

private:
  hertz driver_frequency() noexcept
  {
    return m_frequency;
  }

  result<std::uint64_t> driver_uptime() noexcept
  {
    if (m_uptime_values.size() == 0) {
      return hal::new_error(std::out_of_range("uptimes queue is empty!"));
    }
    std::uint64_t m_current_value = m_uptime_values.front();
    m_uptime_values.pop();
    return m_current_value;
  }

  hertz m_frequency = 1.0_Hz;
  std::queue<std::uint64_t> m_uptime_values{};
};
/** @} */
}  // namespace hal::mock
