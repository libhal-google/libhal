/**
 * @file timeout.hpp
 * @brief Provides the hal::timeout type and utility functions that use that
 * type.
 *
 */
#pragma once

#include <functional>

#include "error.hpp"

namespace hal {
/**
 * @brief Represents the state of a coroutine or resumable callable
 *
 */
enum class work_state
{
  // Callable is in progress and has not yet finished performing its work.
  in_progress,
  // Callable was able to determine that it failed to do what it was tasked to
  // do and has terminated.
  failed,
  // Callable finished the work it needed to perform and has terminated.
  finished,
};

/**
 * @brief Timeout is a callable object or function that signals to a procedure
 * that the procedure has exceeded its time allotment and should return control
 * to the calling function.
 *
 * @throws hal::timeout - when the timeout condition has been met.
 * @returns status - sets error flag set when timeout
 * condition has been met, otherwise returns success.
 */
using timeout_function = status(void);

template<class T>
concept timeout = std::convertible_to<T, std::function<timeout_function>>;

/**
 * @brief A non-blocking callable that performs work with each call
 *
 * Each call to a work_function will perform a set of work. The worker will
 * return a work_state to indicate its current state. Once the worker reaches a
 * terminal state, it MUST perform no additional work and return the terminal
 * state. For example, if a work function failed, it must always return failure
 * and not interact with hardware or other software from that point on. Same
 * will occur for the "finished" state.
 *
 * This function can be repeatedly tried until it has reached a terminal state
 * with the try_until() function.
 *
 * @returns result<work_state> - sets error flag set when an error occurs,
 * otherwise returns work_state enum.
 */
using work_function = result<work_state>();

template<class T>
concept worker = std::convertible_to<T, std::function<work_function>>;

/**
 * @brief Delay the execution of the application or thread for a duration of
 * time.
 *
 * @tparam Timeout - timeout type
 * @param p_timeout - callable timeout object
 * @return status - success or failure
 */
[[nodiscard]] inline status delay(timeout auto p_timeout) noexcept
{
  bool waiting = true;

  // This lambda catches a `std::errc::timed_out` handle them by changing
  // `waiting` from true to false in order to break the while loop below.
  auto timeout_catcher =
    [&waiting](hal::match<std::errc, std::errc::timed_out> p_errc) -> status {
    (void)p_errc;
    // Simply change the waiting bool
    waiting = false;
    // return successful
    return {};
  };

  HAL_CHECK(hal::attempt(
    [&p_timeout]() -> status {
      // Continuously call p_callback until it either returns
      // `std::errc::timeout_out`
      while (true) {
        HAL_CHECK(p_timeout());
      }
      // Unreachable!
      return {};
    },
    timeout_catcher));

  return {};
}

/**
 * @brief Create a timeout that will never time out
 *
 * @return auto - callable that will never return timeout
 */
[[nodiscard]] inline auto never_timeout() noexcept
{
  return []() -> status { return {}; };
}
}  // namespace hal
