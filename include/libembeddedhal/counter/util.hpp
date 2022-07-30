#pragma once

#include <chrono>
#include <system_error>

#include "../error.hpp"
#include "../timeout.hpp"
#include "../units.hpp"
#include "interface.hpp"
#include "timeout.hpp"

namespace hal {
/**
 * @addtogroup counter
 * @{
 */
/**
 * @brief Create a timeout object based on hal::counter.
 *
 * NOTE: that multiple timeout objects can be made from a single counter without
 * any influence on other timeout objects.
 *
 * @param p_counter - hal::counter implementation
 * @param p_duration - amount of time until timeout
 * @return result<hal::counter_timeout>
 */
inline result<hal::counter_timeout> create_timeout(
  hal::counter& p_counter,
  hal::time_duration p_duration)
{
  return hal::counter_timeout::create(p_counter, p_duration);
}

/**
 * @brief Delay execution for a duration of time using a hardware counter.
 *
 * @param p_counter - counter driver
 * @param p_duration - the amount of time to delay for, must be positive
 * @return status - returns any errors that result from
 * hal::counter::uptime(), otherwise returns success.
 * @throws std::errc::result_out_of_range - if the calculated cycle count
 * exceeds std::int64_t.
 */
[[nodiscard]] inline status delay(hal::counter& p_counter,
                                  hal::time_duration p_duration) noexcept
{
  if (p_duration < hal::time_duration(0)) {
    return boost::leaf::new_error(std::errc::result_out_of_range);
  }
  auto timeout_object = HAL_CHECK(create_timeout(p_counter, p_duration));
  return delay(timeout_object);
}
/** @} */
}  // namespace hal
