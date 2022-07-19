#pragma once

#include <queue>

#include "interface.hpp"

namespace embed::mock {
/**
 * @addtogroup counter
 * @{
 */
/**
 * @brief mock counter implementation for use in unit tests and
 * simulations.
 *
 */
struct counter : public embed::counter_interface
{
  /**
   * @brief Queues the uptimes to be returned for uptime()
   *
   * @param p_uptimes - queue of uptimes
   */
  void set(std::queue<uptime_t>& p_uptimes)
  {
    m_uptimes = p_uptimes;
  }

private:
  boost::leaf::result<uptime_t> driver_uptime() noexcept
  {
    if (m_uptimes.size() == 0) {
      return boost::leaf::new_error(
        std::out_of_range("uptimes queue is empty!"));
    }
    uptime_t m_current_value = m_uptimes.front();
    m_uptimes.pop();
    return m_current_value;
  };

  std::queue<uptime_t> m_uptimes{};
};
/** @} */
}  // namespace embed::mock
