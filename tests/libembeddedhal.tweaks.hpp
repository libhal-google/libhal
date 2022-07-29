#pragma once

namespace embed::config {
inline int callback_call_count = 0;
constexpr bool on_error_callback_enabled = true;
constexpr auto on_error_callback = []() { callback_call_count++; };
}  // namespace embed::config
