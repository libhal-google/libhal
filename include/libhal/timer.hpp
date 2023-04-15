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

#pragma once

#include <chrono>

#include "config.hpp"
#include "error.hpp"
#include "functional.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief Timer hardware abstraction interface.
 *
 * Use this interface for devices and peripherals that have timer like
 * capabilities, such that, when a timer's time has expired, an event,
 * interrupt, or signal is generated.
 *
 * Timer drivers tick period must be an integer multiple of 1 nanosecond,
 * meaning that the only tick period allowed are 1ns, 2ns, up to the maximum
 * holdable in a std::chrono::nanosecond type. sub-nanosecond tick periods are
 * not allowed.
 *
 */
class timer
{
public:
  /**
   * @brief Error type indicating that the desired time delay is not achievable
   * with this timer.
   *
   * This occurs if the time delay is too large based on the tick period of the
   * timer.
   */
  struct out_of_bounds_error
  {
    /// The tick period
    std::chrono::nanoseconds tick_period;
    /// The maximum possible delay allowed
    std::chrono::nanoseconds maximum;
  };

  /**
   * @brief Feedback after checking if the timer is running.
   *
   */
  struct is_running_t
  {
    /**
     * @brief Determines if the timer is currently running
     *
     * If this value is false, then the timer is not running.
     * If this value is true, then the timer is currently running and a callback
     * is scheduled to be executed at some point in the future.
     *
     */
    bool is_running;
  };

  /**
   * @brief Feedback from cancelling a timer
   *
   * This structure is currently empty as no feedback has been determined for
   * now. This structure may be expanded in the future.
   */
  struct cancel_t
  {};

  /**
   * @brief Feedback from scheduling a timer
   *
   * This structure is currently empty as no feedback has been determined for
   * now. This structure may be expanded in the future.
   */
  struct schedule_t
  {};

  /**
   * @brief Determine if the timer is currently running
   *
   * @return result<is_running_t> - information about the timer
   */
  [[nodiscard]] result<is_running_t> is_running()
  {
    return driver_is_running();
  }

  /**
   * @brief Stops a scheduled event from happening.
   *
   * Does nothing if the timer is not currently running.
   *
   * Note that there must be sufficient time between the this call finishing and
   * the scheduled event's termination. If this call is too close to when the
   * schedule event expires, this function may not complete before the hardware
   * calls the callback.
   *
   * @return result<cancel_t> - success or failure
   */
  [[nodiscard]] result<cancel_t> cancel()
  {
    return driver_cancel();
  }

  /**
   * @brief Schedule an callback be be executed after the delay time
   *
   * If this is called and the timer has already scheduled an event (in other
   * words, `is_running()` returns true), then the previous scheduled event will
   * be canceled and the new scheduled event will be started.
   *
   * If the delay time result in a tick period of 0, then the timer will execute
   * after 1 tick period. For example, if the tick period is 1ms and the
   * requested time delay is 500us, then the event will be scheduled for 1ms.
   *
   * If the tick period is 1ms and the requested time is 2.5ms then the event
   * will be scheduled after 2 tick periods or in 2ms.
   *
   * @param p_callback - callback function to be called when the timer expires
   * @param p_delay - the amount of time until the timer expires
   * @return result<schedule_t> - success or failure
   * @throws out_of_bounds_error - if p_interval is greater than what can be
   * cannot be achieved
   */
  [[nodiscard]] result<schedule_t> schedule(
    hal::callback<void(void)> p_callback,
    hal::time_duration p_delay)
  {
    return driver_schedule(p_callback, p_delay);
  }

  virtual ~timer() = default;

private:
  virtual result<is_running_t> driver_is_running() = 0;
  virtual result<cancel_t> driver_cancel() = 0;
  virtual result<schedule_t> driver_schedule(
    hal::callback<void(void)> p_callback,
    hal::time_duration p_delay) = 0;
};
}  // namespace hal
