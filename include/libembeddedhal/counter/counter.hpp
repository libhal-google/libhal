#pragma once

#include <chrono>
#include <cstdint>

#include "../error.hpp"
#include "../frequency.hpp"

namespace embed {
/**
 * @brief Counter hardware abstraction interface. Use this interface for devices
 * and peripherals that have counting capabilities. Such devices can count up or
 * down. Useful for keeping time. Expect counters to overflow when they reach
 * the end of counting register's limits.
 *
 */
class counter
{
public:
  /**
   * @brief Enumerations listing out the potential errors a counter could
   * encounter
   *
   */
  enum class errors
  {
    /// The counter has back tracked and a previous value is greater than the
    /// current value. This is an invalid state for a hardware counter and
    /// considered an error.
    backtrack,
  };

  /**
   * @brief Returns the operating frequency of the counter.
   *
   * @return boost::leaf::result<embed::frequency> - the operating frequency of
   * the counter.
   */
  [[nodiscard]] boost::leaf::result<embed::frequency> frequency() noexcept
  {
    return driver_frequency();
  }
  /**
   * @brief Get the uptime of the counter since it has started.
   *
   * The count for a counter must always count up.
   *
   * Most counters are only support 32-bits or lower. In this case there are
   * multiple way to increase the bit width of the counter:
   *
   *  1. Use the overflow_count class to detect overflows each time the count is
   *     called. In cases where this is used, the counter::count() must be
   *     called often enough to detect overflows
   *  2. Use the overflow_count class with overflow interrupt. This will
   *     eliminate the need to call the count often enough to detect the
   *     overflows.
   *
   * @return std::chrono::nanoseconds - the current uptime since the counter has
   * started.
   */
  [[nodiscard]] boost::leaf::result<std::uint64_t> uptime() noexcept
  {
    return driver_uptime();
  }

private:
  virtual boost::leaf::result<std::uint64_t> driver_uptime() noexcept = 0;
  virtual boost::leaf::result<embed::frequency> driver_frequency() noexcept = 0;
};
}  // namespace embed
