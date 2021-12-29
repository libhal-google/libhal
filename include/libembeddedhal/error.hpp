#pragma once

#include "internal/third_party/leaf.hpp"

namespace embed::error {
template<typename T>
constexpr auto&& get_return_value(boost::leaf::result<T>& p_result)
{
  if constexpr (std::is_void_v<T>) {
    return 0;
  } else {
    return p_result.value();
  }
}
}  // namespace embed::error

#define EMBED_CHECK(expr)                                                      \
  ({                                                                           \
    auto&& temp = (expr);                                                      \
    if (!temp) {                                                               \
      return temp.error();                                                     \
    }                                                                          \
    ::embed::error::get_return_value(temp);                                    \
  })

#define EMBED_ENABLE_TRACE()
