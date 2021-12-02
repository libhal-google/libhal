#pragma once

#include <algorithm>
#include <cinttypes>
#include <climits>
#include <concepts>
#include <cstddef>
#include <limits>
#include <type_traits>

namespace embed {
/**
 * @brief
 *
 * @tparam bit_width
 * @tparam int_t
 */
template<uint8_t bit_width, typename int_t>
class bit_limits
{
public:
  // Check that the int_t is actually an integer.
  static_assert(
    std::is_integral_v<int_t>,
    "Type must be an integral type like int8_t, uint16_t, int32_t, etc!");

  // Check that the bit width is less than or equal to the size of the int_t.
  static_assert(
    bit_width <= sizeof(int_t) * 8,
    "The bit_width exceed the number of bit_width in the integer type.");

  // Check that bit width is not zero.
  static_assert(bit_width != 0, "The bit_width cannot be 0.");

  /// @return constexpr int_t - returns the maximum value available for an
  /// integer of `bit_width` size and that can be stored within `int_t`. The
  /// final value of the function depends also on the sign of the int type.

  /**
   * @brief Get the maximum value available for an integer of `bit_width` size
   * and that can be stored within `int_t`. The final value of the function
   * depends also on the sign of the int type.
   *
   * @return constexpr int_t maximum value
   */
  static constexpr int_t max()
  {
    if constexpr (bit_width == 64) {
      return std::numeric_limits<int_t>::max();
    } else if constexpr (std::is_signed_v<int_t>) {
      int_t max = static_cast<int_t>(((1ULL << bit_width) / 2ULL) - 1ULL);
      return max;
    } else {
      int_t max = static_cast<int_t>((1ULL << bit_width) - 1ULL);
      return max;
    }
  }

  /**
   * @brief Get the minimum value available for an integer of `bit_width` size
   * and that can be stored within `int_t`. The final value of the function
   * depends also on the sign of the int type. Unsigned ints simply return zero.
   *
   * @return constexpr int_t minimum value
   */
  static constexpr int_t min()
  {
    if constexpr (bit_width == 64) {
      return std::numeric_limits<int_t>::min();
    } else if constexpr (std::is_signed_v<int_t>) {
      int_t min = static_cast<int_t>(((1ULL << bit_width) / 2ULL) * -1ULL);
      return min;
    } else {
      return 0U;
    }
  }
};

/**
 * @brief Generate a mask of 1s at compiletime
 *
 * @tparam bit_field_width number of 1s in the mask
 * @return consteval uint32_t mask with 1s at the LSB
 */
template<size_t bit_field_width>
static consteval uint32_t generate_field_of_ones()
{
  uint32_t result = 0;
  for (int i = 0; i < bit_field_width; i++) {
    result |= 1 << i;
  }
  return result;
}

/**
 * @brief
 *
 * @tparam T
 * @tparam source_width
 * @tparam U
 * @param p_value
 * @return constexpr T
 */
template<typename T, size_t source_width, typename U>
constexpr static T increase_bit_depth(U p_value)
{
  constexpr size_t output_bit_width = sizeof(T) * CHAR_BIT;

  // Disallow anything other than integral types. This also disallows floats
  // as this type seeks to eliminate their use as much as possible.
  static_assert(std::is_integral_v<T>,
                "Full scale can only be an unsigned integral type .");

  static_assert(output_bit_width >= source_width,
                "The destination bit width must be equal to or greater than "
                "the type of the value to be scaled up.");

  // Calculate the difference between the destination and the source width
  constexpr size_t width_difference = output_bit_width - source_width;

  constexpr size_t final_source_width =
    (std::is_signed_v<T>) ? source_width - 1 : source_width;
  constexpr size_t final_width_difference =
    (std::is_signed_v<T>) ? width_difference - 1 : width_difference;

  T result = static_cast<T>(p_value) << width_difference;

  if (p_value > 0) {
    for (int i = final_source_width; i < final_width_difference;
         i += final_source_width) {
      result |= (result >> i);
    }
  }

  return result;
}

/**
 * @brief
 *
 * @tparam T
 * @tparam bit_width
 */
template<typename T, size_t bit_width>
struct bit_depth
{
  T value = 0;

  constexpr bit_depth(T p_value)
    : value(p_value)
  {
    value = std::clamp(p_value,
                       bit_limits<bit_width, T>::min(),
                       bit_limits<bit_width, T>::max());
  }
};

/**
 * @brief
 *
 * @tparam T
 */
template<typename T>
class full_scale
{
public:
  // Disallow anything other than integral types. This also disallows floats
  // as this type seeks to eliminate their use as much as possible.
  static_assert(std::is_integral_v<T>,
                "Full scale can only be an unsigned integral type .");
  /**
   * @brief Calculate the number
   *
   */
  static constexpr size_t bit_width = sizeof(T) * CHAR_BIT;

  constexpr full_scale()
    : m_value(0)
  {}

  template<typename U, size_t bit_width>
  constexpr full_scale(bit_depth<U, bit_width> p_value)
  {
    *this = p_value;
  }

  template<typename U, size_t bit_width>
  constexpr full_scale<T>& operator=(bit_depth<U, bit_width> p_value)
  {
    m_value = increase_bit_depth<T, bit_width, U>(p_value.value);
    return *this;
  }

  T value() { return m_value; }

private:
  T m_value = 0;
};

/**
 * @brief
 *
 * @tparam T
 * @tparam U
 * @param value
 * @param scale
 * @return auto
 */
template<std::unsigned_integral T, std::unsigned_integral U>
auto operator*(U value, full_scale<T> scale)
{
  std::uintmax_t arith_container = value;
  arith_container = arith_container * scale.value();
  arith_container = arith_container / std::numeric_limits<T>::max();
  return static_cast<U>(arith_container);
}
} // namespace embed
