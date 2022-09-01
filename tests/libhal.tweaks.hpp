#pragma once

namespace hal::config {
constexpr std::string_view platform = "test";
inline int callback_call_count = 0;
constexpr bool on_error_callback_enabled = true;
constexpr auto on_error_callback = []() { callback_call_count++; };
}  // namespace hal::config
