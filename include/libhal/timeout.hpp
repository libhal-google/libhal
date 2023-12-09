// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file timeout.hpp
 * @brief Provides the hal::timeout type and utility functions that use that
 * type.
 *
 */
#pragma once

#include "functional.hpp"

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
 * @brief Signature of a function that throws std::errc::timed_out
 *
 * A function that expires after a certain amount of time or after a certain
 * amount of events, or after a specific event occurs. When called, this
 * function checks if the expiration event has occurred and if so, throws the
 * exception std::errc::timed_out.
 *
 * @throws std::errc::timed_out - to indicate that it has expired
 */
using timeout_function = void(void);

template<class T>
concept timeout = std::convertible_to<T, hal::function_ref<timeout_function>>;

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
 * @returns work_state - indicates what the state of the worker function is.
 */
using work_function = work_state();

template<class T>
concept worker = std::convertible_to<T, hal::function_ref<work_function>>;

/**
 * @brief Delay the execution of the application or thread for a duration of
 * time.
 *
 * @param p_timeout - callable timeout object/function
 */
inline void delay(timeout auto p_timeout)
{
  try {
    while (true) {
      p_timeout();
    }
  } catch (const std::errc& p_error) {
    if (p_error != std::errc::timed_out) {
      throw;
    }
  }

  while (true) {
    // Does stuff ...
    if (!p_timeout()) {
      throw std::errc::timed_out;
    }
  }
}

/**
 * @brief Create a timeout that will never time out
 *
 * @return auto - callable that will never timeout
 */
inline auto never_timeout()
{
  return []() {};
}
}  // namespace hal
