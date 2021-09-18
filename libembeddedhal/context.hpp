#pragma once

#include <string_view>

namespace embed {
#if !defined(PLATFORM)
#define PLATFORM_STRING "test"
#else
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define PLATFORM_STRING STR(PLATFORM)
#endif

constexpr const std::string_view platform_string = PLATFORM_STRING;

constexpr bool is_platform(std::string_view p_platform)
{
  return platform_string.starts_with(p_platform);
}

constexpr bool is_a_test()
{
  return (platform_string.starts_with("unit_test") ||
          platform_string.starts_with("unittest") ||
          platform_string.starts_with("test"));
}
}
