/**
 * @file config.hpp
 * @brief Configuration details for the platform or target the code will be
 * running on.
 *
 * This file is guaranteed to be included when any libhal interface is included.
 *
 * It is REQUIRED that project using libhal defines a `libhal.tweaks.hpp` file
 * for each target a project will run on.
 *
 */

#pragma once

#include <string_view>

namespace hal::config {
namespace defaults {
/**
 * @brief The platform/target that the code is running on
 *
 * This field is required to be changed to the appropriate name of the target as
 * defined in the target library's documentation.
 *
 * Defaults to "undefined". "undefined" is not a valid platform name and can be
 * used to determine if the libhal.tweaks.hpp file is missing from the include
 * list or the value is not set.
 *
 * Setting this to "unit_test", "unittest", or "test" will indicate to all
 * libraries that the code is running in a unit test and should change their
 * behavior for testing or simulation purposes.
 */
constexpr std::string_view platform = "undefined";
/**
 * @brief Enable/disable on error callbacks
 *
 */
constexpr bool on_error_callback_enabled = false;
/**
 * @brief Custom callback that can be called if an error occurs
 *
 * If enabled, this is called right before throwing the error. The information
 * about the type of error and where it originated is not available to this
 * function. The primary use case for this function is to capture information
 * that can only be known at the site of an error. For example, the only way to
 * get a stack trace is to run a stack trace at the point where the stack trace
 * occurred. Registers or other bits of information can also be collected before
 * bubbling up the error which could disturb or change that information.
 *
 * Handling the error SHOULD NOT be done in this callback. This should be
 * reserved for code higher up in the application's call stack.
 *
 */
constexpr auto on_error_callback = []() {};
}  // namespace defaults
using namespace defaults;
}  // namespace hal::config

// Pull the tweaks file and override the hal::config defaults
#if __has_include(<libhal.tweaks.hpp>)
#include <libhal.tweaks.hpp>
#else
#error                                                                         \
  "The 'libhal.tweaks.hpp' file could not be found! This file is required for libhal to work correctly."
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
[[nodiscard]] constexpr bool is_platform(std::string_view p_platform)
{
  return config::platform.starts_with(p_platform);
}

/**
 * @brief Determines if the application is a test
 *
 * @return true - this application is a test
 * @return false - this application is not a test
 */
[[nodiscard]] constexpr bool is_a_test()
{
  return (config::platform.starts_with("unit_test") ||
          config::platform.starts_with("unittest") ||
          config::platform.starts_with("test"));
}
};  // namespace hal
