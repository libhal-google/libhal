#pragma once

namespace hal::config {
inline int callback_call_count = 0;
constexpr bool on_error_callback_enabled = true;
constexpr auto on_error_callback = []() { callback_call_count++; };
// TODO(#404): Change this to "float" when the platform can support it
using float_type = double;
}  // namespace hal::config
