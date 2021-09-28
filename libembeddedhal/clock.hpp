#pragma once

#include <chrono>
#include <functional>

namespace embed {
class this_thread
{
public:
  static void sleep_for(std::chrono::nanoseconds delay)
  {
    m_global_sleep(delay);
  }

  static void set_global_sleep(
    std::function<void(std::chrono::nanoseconds)> sleep_function)
  {
    m_global_sleep = sleep_function;
  }

private:
  static inline auto m_loop_delay = [](std::chrono::nanoseconds loop_count) {
    std::chrono::nanoseconds count{ 0 };
    while (count < loop_count) {
      count += std::chrono::microseconds{ 1 };
    }
  };

  static inline std::function<void(std::chrono::nanoseconds)> m_global_sleep =
    m_loop_delay;
};
} // namespace embed
