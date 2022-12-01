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
 * @addtogroup steady_clock
 * @{
 */
/**
 * @brief Create a timeout object based on hal::steady_clock.
 *
 * NOTE: Multiple timeout objects can be made from a single steady_clock
 * without influencing other timeout objects.
 *
 * @param p_steady_clock - hal::steady_clock implementation
 * @param p_duration - amount of time until timeout
 * @return result<hal::steady_clock_timeout> - timeout object
 */
inline result<hal::steady_clock_timeout> create_timeout(
  hal::steady_clock& p_steady_clock,
  hal::time_duration p_duration)
{
  return hal::steady_clock_timeout::create(p_steady_clock, p_duration);
}

/**
 * @brief Delay execution for a duration of time using a hardware steady_clock.
 *
 * @param p_steady_clock - steady_clock driver
 * @param p_duration - the amount of time to delay for. Zero or negative time
 * duration will delay for one tick of the p_steady_clock.
 * @return status - if successful, then execution of the program was halted for
 * the p_duration time frame, otherwise, a failure occurred either when creating
 * the timeout from the steady clock or from calling uptime() on the steady
 * clock. In the failure situation, it is unknown how long the program/thread
 * halted for.
 */
[[nodiscard]] inline status delay(hal::steady_clock& p_steady_clock,
                                  hal::time_duration p_duration) noexcept
{
  auto timeout_object = HAL_CHECK(create_timeout(p_steady_clock, p_duration));
  return delay(timeout_object);
}

/**
 * @brief Generates a function that, when passed a duration, returns a timeout
 *
 * @param p_steady_clock - steady_clock driver that must out live the lifetime
 * of the returned lambda.
 * @return auto - a callable that returns a new timeout object each time a time
 * duration is passed to it.
 */
inline auto timeout_generator(hal::steady_clock& p_steady_clock)
{
  return [&p_steady_clock](hal::time_duration p_duration) -> auto
  {
    return create_timeout(p_steady_clock, p_duration);
  };
}
/** @} */
}  // namespace hal
