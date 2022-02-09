#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <string_view>

namespace embed {
/**
 * @brief Convert a string_view into a std::array of N number of characters.
 *
 * Will always ensure that the array is null terminated
 *
 * @tparam N - Size of the array
 * @param p_view - string to be placed into a char array
 * @return constexpr std::array<char, N + 1> - the char array object
 */
template<size_t N>
[[nodiscard]] constexpr std::array<char, N + 1> to_array(
  std::string_view p_view) noexcept
{
  const size_t min = std::min(N, p_view.size());
  std::array<char, N + 1> result;

  auto out_iterator = std::copy_n(p_view.begin(), min, result.begin());
  std::fill(out_iterator, result.end(), '\0');

  return result;
}
}  // namespace embed
