
#pragma once

#include <cstdint>

#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief Hardware abstraction interface for a steady clock mechanism
 *
 * Implementations of this interface must follow the same requirements as a
 * std::chrono::steady_clock, in that the clock is monotonic & steady.
 *
 * This clock is steady meaning that subsequent calls to get the uptime of this
 * clock cannot decrease as physical time moves forward and the time between
 * ticks of this clock are constant and defined by the clock's frequency.
 *
 * This can be used to get the time since the boot up, or to be more accurate,
 * the time when the steady clock object is created. This clock is most suitable
 * for measuring time intervals.
 *
 * After creation of the clock, the operating frequency will not change.
 *
 */
class steady_clock
{
public:
  /**
   * @return hertz - operating frequency of the steady clock. Will always return
   * a positive value.
   */
  [[nodiscard]] hertz frequency() noexcept
  {
    return driver_frequency();
  }

  /**
   * @return result<std::uint64_t> - the number of ticks of the steady clock
   * since its creation.
   */
  [[nodiscard]] result<std::uint64_t> uptime() noexcept
  {
    return driver_uptime();
  }

  virtual ~steady_clock() = default;

private:
  virtual hertz driver_frequency() noexcept = 0;
  virtual result<std::uint64_t> driver_uptime() noexcept = 0;
};
}  // namespace hal
