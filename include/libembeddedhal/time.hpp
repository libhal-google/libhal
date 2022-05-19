#pragma once

#include <chrono>
#include <functional>

#include "error.hpp"

namespace embed::this_thread {
/// Smallest increment of time for clocks
using time_period = std::chrono::nanoseconds;
/// Definition of a sleep function
using sleep_function = std::function<void(time_period p_sleep_time)>;
/// Definition of an uptime function
using uptime_function = std::function<time_period(void)>;

/**
 * @brief structure containing internal clock variables and functions
 *
 */
struct global_clocks
{
public:
  /**
   * @brief Default uptime timer that simply counts each time it is called. This
   * should almost never be used.
   *
   * @return time_period - fake uptime
   */
  static time_period incremental_uptime() noexcept
  {
    using namespace std::chrono_literals;
    static time_period count{ 0 };
    count += 100ns;
    return count;
  }
  /**
   * @brief Default sleep function that simply counts until it reaches a
   * designated number.
   *
   * @param p_loop_count - number of iterations to loop for to mimic sleeping
   */
  static void loop_sleep(time_period p_loop_count) noexcept
  {
    using namespace std::chrono_literals;
    time_period count{ 0 };
    while (count < p_loop_count) {
      count += 100us;
    }
  }
  /**
   * @brief Store the global sleep function
   *
   */
  static inline sleep_function m_global_sleep{};
  /**
   * @brief Store the global uptime function
   *
   */
  static inline uptime_function m_global_uptime{};
};

/**
 * @brief Delay execution of the program by a predefined time
 *
 * @param p_delay - the amount of time to delay execution by
 */
inline void sleep_for(time_period p_delay) noexcept
{
  if (global_clocks::m_global_sleep) {
    global_clocks::m_global_sleep(p_delay);
  } else {
    global_clocks::loop_sleep(p_delay);
  }
}
/**
 * @brief Get the current system uptime
 *
 * @return auto - the global uptime
 */
[[nodiscard]] inline auto uptime() noexcept
{
  if (global_clocks::m_global_uptime) {
    return global_clocks::m_global_uptime();
  } else {
    return global_clocks::incremental_uptime();
  }
}
/**
 * @brief Set the global sleep object
 *
 * @param p_sleep_function - the function to handle sleeping
 */
inline void set_global_sleep(sleep_function p_sleep_function) noexcept
{
  global_clocks::m_global_sleep = p_sleep_function;
}
/**
 * @brief Set the global uptime object
 *
 * @param p_uptime_function - the function to return the current system uptime
 */
inline void set_global_uptime(uptime_function p_uptime_function) noexcept
{
  global_clocks::m_global_uptime = p_uptime_function;
}
}  // namespace embed::this_thread