#include <chrono>
#include <thread>

#include <libembeddedhal/time.hpp>

void unit_test_sleep(std::chrono::nanoseconds p_sleep_time) {
  std::this_thread::sleep_for(p_sleep_time);
}

std::chrono::nanoseconds unit_test_uptime() {
  return std::chrono::steady_clock::now().time_since_epoch();
}

int main() {
  embed::this_thread::set_global_sleep(unit_test_sleep);
  embed::this_thread::set_global_uptime(unit_test_uptime);
  embed::this_thread::uptime();
  embed::this_thread::sleep_for(std::chrono::nanoseconds(0));
}