#pragma once

#include <string_view>

namespace hal::config {
namespace defaults {
constexpr std::string_view platform = "test";
constexpr bool on_error_callback_enabled = false;
constexpr auto on_error_callback = []() {};
}  // namespace defaults
using namespace defaults;
}  // namespace hal::config

// Pull the tweaks
#if __has_include(<libembeddedhal.tweaks.hpp>)
#include <libembeddedhal.tweaks.hpp>
#endif

namespace hal {
/**
 * @brief Determines if the current application was built for a specific
 * platform. For example:
 *
 *    hal::is_platform("lpc4078");
 *
 * Will return true if the PLATFORM macro defined at compile time was equal to
 * lpc4078. If the developer wants to be less specific, let say, to just
 * determine if the platform is in the lpc40xx family then the following example
 * will work.
 *
 *    hal::is_platform("lpc40");
 *
 * @param p_platform - platform string pattern to check against
 * @return true - matches the platform string
 * @return false - does not matches the platform string
 */
[[nodiscard]] constexpr bool is_platform(std::string_view p_platform) noexcept
{
  return config::platform.starts_with(p_platform);
}

/**
 * @brief Determines if the application is a test
 *
 * @return true - this application is a test
 * @return false - this application is not a test
 */
[[nodiscard]] constexpr bool is_a_test() noexcept
{
  return (config::platform.starts_with("unit_test") ||
          config::platform.starts_with("unittest") ||
          config::platform.starts_with("test"));
}
};  // namespace hal