#pragma once

#include <algorithm>
#include <array>
#include <span>

template<typename T, size_t size>
inline bool operator==(const std::array<T, size>& p_array,
                       const std::span<T>& p_span) noexcept
{
  if (p_span.size() != size) {
    return false;
  }

  return std::equal(p_array.begin(), p_array.end(), p_span.begin());
}

template<typename T, size_t size>
inline bool operator!=(const std::array<T, size>& p_array,
                       const std::span<T>& p_span) noexcept
{
  return !(p_array == p_span);
}
