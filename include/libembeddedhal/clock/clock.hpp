#pragma once

#include <chrono>
#include <functional>

namespace embed::this_thread {
/**
 * @brief smallest increment of time for clocks
 *
 */
using time_increment = std::chrono::nanoseconds;
/**
 * @brief definition of a sleep function
 *
 */
using sleep_function = std::function<void(std::chrono::nanoseconds)>;
/**
 * @brief definition of an uptime function
 *
 */
using uptime_function = std::function<std::chrono::nanoseconds(void)>;

/**
 * @brief internal clock
 *
 */
struct global_clocks
{
public:
  /**
   * @brief Default uptime timer that simply counts each time it is called. This
   * should almost never be used.
   *
   */
  static inline auto m_incremental_uptime = []() -> time_increment {
    using namespace std::chrono_literals;
    static time_increment count{ 0 };
    count += 100ns;
    return count;
  };
  /**
   * @brief Default sleep function that simply counts until it reaches a
   * designated number.
   *
   */
  static inline auto m_loop_delay = [](time_increment loop_count) {
    time_increment count{ 0 };
    while (count < loop_count) {
      count += std::chrono::microseconds{ 100 };
    }
  };
  /**
   * @brief Store the global sleep function
   *
   */
  static inline sleep_function m_global_sleep = m_loop_delay;
  /**
   * @brief Store the global uptime function
   *
   */
  static inline uptime_function m_global_uptime = m_incremental_uptime;
};

/**
 * @brief Delay execution of the program by a predefined time
 *
 * @param delay the amount of time to delay execution by
 */
static auto sleep_for(time_increment delay)
{
  return global_clocks::m_global_sleep(delay);
}
/**
 * @brief Get the current system uptime
 *
 * @return auto the global uptime
 */
static auto uptime()
{
  return global_clocks::m_global_uptime();
}

/**
 * @brief Set the global sleep object
 *
 * @param sleep_function the function to handle sleeping
 */
static void set_global_sleep(sleep_function sleep_function)
{
  global_clocks::m_global_sleep = sleep_function;
}
/**
 * @brief Set the global uptime object
 *
 * @param uptime_function the function to return the current system uptime
 */
static void set_global_uptime(uptime_function uptime_function)
{
  global_clocks::m_global_uptime = uptime_function;
}
} // namespace embed::this_thread
