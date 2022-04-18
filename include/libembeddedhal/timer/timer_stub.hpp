#include "timer.hpp"

namespace embed::stub {
/**
 * @brief A stub implementation of embed::timer to be used as a placeholder for
 * drivers that require an optional timer. Calling clear() will make
 * is_running() return false, calling schedule() will make is_running() return
 * true.
 *
 */
class timer : public embed::timer
{
private:
  boost::leaf::result<bool> driver_is_running() noexcept
  {
    return m_is_running;
  }

  boost::leaf::result<void> driver_clear() noexcept
  {
    m_is_running = false;
    return {};
  }

  boost::leaf::result<void> driver_schedule(
    [[maybe_unused]] std::function<void(void)> p_callback,
    [[maybe_unused]] std::chrono::nanoseconds p_delay) noexcept
  {
    m_is_running = true;
    return {};
  }
  bool m_is_running = false;
};
}  // namespace embed::stub
