#pragma once

#include "interface.hpp"

namespace embed::experimental {
/**
 * @addtogroup counter
 * @{
 */
/**
 * @brief uptime counter takes a hardware counter and calculates the uptime in
 * nanoseconds.
 *
 * This class should be used over calling frequency::duration_from_cycles() to
 * calculate uptime because that function will overflow when 9223372 seconds or
 * around 15 weeks have elapsed. This has to do with how the integer arithmetic
 * is handled for converting from frequency to duration using a cycle count.
 *
 * So long as this class's uptime() is at least once every 15 weeks, the uptime
 * will be accurate up to ~292 years.
 */
class uptime_counter
{
public:
  /**
   * @brief Construct a new uptime counter object
   *
   * @param p_counter - hardware counter
   */
  uptime_counter(counter& p_counter)
    : m_counter(&p_counter)
  {
  }

  /**
   * @brief Calculates the number of nanoseconds since the counter has started
   *
   * To get the correct uptime, this function must be called within a 15
   * week time period to hold accuracy.
   *
   * @return boost::leaf::result<std::chrono::nanoseconds> - returns the
   */
  boost::leaf::result<std::chrono::nanoseconds> uptime()
  {
    const auto [frequency, new_uptime] = BOOST_LEAF_CHECK(m_counter->uptime());
    auto time_delta =
      static_cast<decltype(m_previous_count)>(new_uptime - m_previous_count);
    auto nanosecond_delta =
      BOOST_LEAF_CHECK(frequency.duration_from_cycles(time_delta));
    m_last_uptime += nanosecond_delta;
    m_previous_count = new_uptime;
    return m_last_uptime;
  }

private:
  embed::counter_interface* m_counter = nullptr;
  uint32_t m_previous_count{};
  std::chrono::nanoseconds m_last_uptime{};
};
/** @} */
}  // namespace embed::experimental
