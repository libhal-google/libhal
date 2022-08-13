#pragma once

#include <queue>

#include "interface.hpp"

namespace hal::mock {
/**
 * @brief mock accelerometer implementation for use in unit tests and
 * simulations.
 *
 */
struct accelerometer : public hal::accelerometer
{
  /**
   * @brief Queues the samples to be returned for read()
   *
   * @param p_samples - queue of samples
   */
  void set(std::queue<sample>& p_samples)
  {
    m_samples = p_samples;
  }

private:
  result<sample> driver_read() noexcept
  {
    if (m_samples.size() == 0) {
      return hal::new_error(std::out_of_range("samples queue is empty!"));
    }
    sample m_current_value = m_samples.front();
    m_samples.pop();
    return m_current_value;
  }

  std::queue<sample> m_samples{};
};
}  // namespace hal::mock
