
#pragma once

#include <chrono>
#include <cstdint>

#include "../error.hpp"
#include "../frequency.hpp"

namespace embed {
/**
 * @addtogroup counter
 * Available hardware counter APIs
 * @{
 *
 */
/**
 * @brief Hardware Counter abstraction interface
 *
 * Use this interface for devices and peripherals that have counting
 * capabilities. Such devices can count up or down. Useful for keeping time.
 * Expect counters to overflow when they reach the end of counting register's
 * limits.
 *
 */
class counter
{
public:
  /**
   * @brief Representation of a counter's uptime
   *
   */
  struct uptime_t
  {
    /// Current operating frequency of the counter
    embed::frequency frequency;
    /// The current count of the counter
    std::uint32_t count;
  };

  /**
   * @brief Get the uptime of the counter since it has started.
   *
   * The count for a counter will always count up.
   * Hardware counters must support 32-bit counts.
   *
   * @return uptime_t - the current uptime since the counter has started.
   */
  [[nodiscard]] boost::leaf::result<uptime_t> uptime() noexcept
  {
    return driver_uptime();
  }

private:
  virtual boost::leaf::result<uptime_t> driver_uptime() noexcept = 0;
};
/** @} */
}  // namespace embed
