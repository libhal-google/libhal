#pragma once

#include <chrono>
#include <functional>

namespace embed {
class this_thread
{
public:
  using time_increment = std::chrono::nanoseconds;
  using sleep_function = std::function<void(std::chrono::nanoseconds)>;
  using uptime_function = std::function<std::chrono::nanoseconds(void)>;

  static auto sleep_for(time_increment delay) { return m_global_sleep(delay); }
  static auto uptime() { return m_global_uptime(); }

  static void set_global_sleep(sleep_function sleep_function)
  {
    m_global_sleep = sleep_function;
  }
  static void set_global_uptime(uptime_function uptime_function)
  {
    m_global_uptime = uptime_function;
  }

private:
  static inline auto m_incremental_uptime = []() -> time_increment {
    static time_increment count{ 0 };
    return count++;
  };
  static inline auto m_loop_delay = [](time_increment loop_count) {
    time_increment count{ 0 };
    while (count < loop_count) {
      count += std::chrono::microseconds{ 1 };
    }
  };

  static inline sleep_function m_global_sleep = m_loop_delay;
  static inline uptime_function m_global_uptime = m_incremental_uptime;
};
} // namespace embed
