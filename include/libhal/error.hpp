#pragma once

#include <system_error>

#include "config.hpp"
#include "internal/third_party/leaf.hpp"

#define HAL_CHECK BOOST_LEAF_CHECK

namespace hal {

template<class T>
using result = boost::leaf::result<T>;
using status = result<void>;

template<class TryBlock, class... H>
[[nodiscard]] constexpr auto attempt(TryBlock&& p_try_block, H&&... p_handlers)
{
  return boost::leaf::try_handle_some(p_try_block, p_handlers...);
}

template<class TryBlock, class... H>
[[nodiscard]] constexpr auto attempt_all(TryBlock&& p_try_block,
                                         H&&... p_handlers)
{
  return boost::leaf::try_handle_all(p_try_block, p_handlers...);
}

template<class... Item>
inline auto new_error(Item&&... p_item) noexcept
{
  if constexpr (config::on_error_callback_enabled) {
    config::on_error_callback();
  }

  return boost::leaf::new_error(std::forward<Item>(p_item)...);
}

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
{
};
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
