#pragma once

#include <system_error>

#include "../frequency.hpp"
#include "../units.hpp"
#include "interface.hpp"

namespace hal {
/**
 * @addtogroup counter
 * @{
 */
/**
 * @brief Timeout object based on hal::counter
 *
 * Prefer to use `hal::create_timeout(hal::counter&)` instead of
 * instantiating this class directly.
 *
 */
class counter_timeout
{
public:
  /**
   * @brief Create a counter_timeout
   *
   * @return boost::leaf::result<counter_timeout> - counter_timeout object
   * @throws std::errc::result_out_of_range if time duration is negative
   */
  static boost::leaf::result<counter_timeout> create(
    hal::counter& p_counter,
    hal::time_duration p_duration)
  {
    if (p_duration < hal::time_duration(0)) {
      return boost::leaf::new_error(std::errc::result_out_of_range);
    }

    const auto [frequency, count] = BOOST_LEAF_CHECK(p_counter.uptime());
    auto cycles = cycles_per(frequency, p_duration);

    return counter_timeout(p_counter, cycles);
  }

  /**
   * @brief Construct a new counter timeout object
   *
   * @param p_timeout - other counter_timeout
   */
  counter_timeout(const counter_timeout& p_timeout) noexcept = default;
  /**
   * @brief Assign construct a new counter timeout object
   *
   * @param p_timeout - other counter_timeout
   * @return counter_timeout&
   */
  counter_timeout& operator=(const counter_timeout& p_timeout) noexcept =
    default;
  /**
   * @brief Construct a new counter timeout object
   *
   * @param p_timeout - other counter_timeout
   */
  counter_timeout(counter_timeout&& p_timeout) noexcept = default;
  /**
   * @brief Move assign construct a new counter timeout object
   *
   * @param p_timeout - other counter_timeout
   * @return counter_timeout&
   */
  counter_timeout& operator=(counter_timeout&& p_timeout) noexcept = default;

  /**
   * @brief Call this object to check if it has timed out.
   *
   * @return boost::leaf::result<void>
   * @throws std::errc::timed_out - if the number of cycles until timeout has
   * been exceeded.
   */
  boost::leaf::result<void> operator()() noexcept
  {
    auto current_count = BOOST_LEAF_CHECK(m_counter->uptime()).count;
    std::uint32_t delta_count = current_count - m_previous_count;
    m_cycles_until_timeout -= delta_count;

    if (m_cycles_until_timeout <= 0) {
      return boost::leaf::new_error(std::errc::timed_out);
    }

    m_previous_count = current_count;

    return {};
  }

private:
  /**
   * @brief Construct a new counter timeout object
   *
   * @param p_counter - counter driver
   * @param p_cycles_until_timeout - number of cycles until timeout
   */
  counter_timeout(hal::counter& p_counter,
                  std::int64_t p_cycles_until_timeout) noexcept
    : m_counter(&p_counter)
    , m_cycles_until_timeout(p_cycles_until_timeout)
  {
  }

  hal::counter* m_counter;
  std::int64_t m_cycles_until_timeout = 0;
  std::uint32_t m_previous_count = 0;
};
/** @} */
}  // namespace hal
