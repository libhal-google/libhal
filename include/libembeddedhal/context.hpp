#pragma once

#include <string_view>

namespace embed {
#if !defined(PLATFORM)
#define PLATFORM_STRING "test"
#warning "No platform specified, defaulting to 'test' platform"
#else
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define PLATFORM_STRING STR(PLATFORM)
#endif

/**
 * @brief The platform string determines which platform this application was
 * built for. Use this to determine at compile or runtime if the running
 * application is a "test", an "lpc40" series mcu, and "stm" series mcu or a
 * raspberry pi ("rpi").
 *
 */
constexpr const std::string_view platform_string = PLATFORM_STRING;

/**
 * @brief Determines if the current application was built for a specific
 * platform. For example:
 *
 *    embed::is_platform("lpc4078");
 *
 * Will return true if the PLATFORM macro defined at compile time was equal to
 * lpc4078. If the developer wants to be less specific, let say, to just
 * determine if the platform is in the lpc40xx family then the following example
 * will work.
 *
 *    embed::is_platform("lpc40");
 *
 * @param p_platform platform string pattern to check against
 * @return true matches the platform string
 * @return false does not matches the platform string
 */
constexpr bool is_platform(std::string_view p_platform)
{
  return platform_string.starts_with(p_platform);
}

/**
 * @brief Determines if the application is a test
 *
 * @return true this application is a test
 * @return false this application is not a test
 */
constexpr bool is_a_test()
{
  return (platform_string.starts_with("unit_test") ||
          platform_string.starts_with("unittest") ||
          platform_string.starts_with("test"));
}
}  // namespace embed
