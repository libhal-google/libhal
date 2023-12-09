// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <system_error>
#include <type_traits>

namespace hal {

using error_handler = void(void);

inline error_handler* on_error_callback = nullptr;

template<class thrown_t>
void safe_throw(thrown_t&& p_thrown_object)
{
  static_assert(
    std::is_trivially_destructible_v<thrown_t>,
    "safe_throw() only works with trivially destructible thrown types");

  if (on_error_callback) {
    on_error_callback();
  }

  throw p_thrown_object;
}

[[noreturn]] inline void halt()
{
  while (true) {
    continue;
  }
}

/**
 * @brief Error objects, templates, and constants.
 *
 */
namespace error {
/**
 * @brief Used for defining static_asserts that should always fail, but only if
 * the static_assert line is hit via `if constexpr` control block. Prefer to NOT
 * use this directly but to use `invalid_option` instead
 *
 * @tparam options ignored by the application but needed to create a non-trivial
 * specialization of this class which allows its usage in static_assert.
 */
template<auto... options>
struct invalid_option_t : std::false_type
{};
/**
 * @brief Helper definition to simplify the usage of invalid_option_t.
 *
 * @tparam options ignored by the application but needed to create a non-trivial
 * specialization of this class which allows its usage in static_assert.
 */
template<auto... options>
inline constexpr bool invalid_option = invalid_option_t<options...>::value;
}  // namespace error
}  // namespace hal
