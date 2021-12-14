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
template<uint8_t bit_width, std::integral int_t>
class bit_limits
{
public:
  // Check that the bit width is less than or equal to the size of the int_t.
  static_assert(
    bit_width <= sizeof(int_t) * 8,
    "The bit_width exceed the number of bit_width in the integer type.");

  // Check that bit width is not zero.
  static_assert(bit_width != 0, "The bit_width cannot be 0.");

  /**
   * @brief Get the maximum value available for an integer of `bit_width` size
   * and that can be stored within `int_t`.
   *
   * The final value of the function depends also on the sign of the int type.
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
   * and that can be stored within `int_t`.
   *
   * The final value of the function depends also on the sign of the int type.
   * Unsigned ints simply return zero.
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
 * @tparam bit_field_width - number of 1s in the mask
 * @return consteval uint32_t - mask with 1s at the LSB
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
 * @brief Take a value of arbitrary bit resolution and create a value with
 * scaled up the bit resolution.
 *
 * The purpose of bit scaling a value is to take a value of lower bit
 * resolution, scale it up but keep the percentage relative to the bits
 * resolution. This is useful for bit resolution erasure as well as image
 * upscaling.
 *
 * For example, lets take an 8-bit value of 127 (or 0x7F). This value
 * is 50% of an 8-bit number. 50% of an 32-bit would be 2147483647 or 0x7FFFFFFF
 * which is half of 2^32. A perfect upscaling would take 0x7F and generate
 * 0x7FFFFFFF in this case. Doing so is quite costly and requires multiplication
 * and division operations which are slower opetation.
 *
 * Fast and efficient bit scaling is done via bit replication. For example, to
 * scale an 8-bit value up to 32-bits would look like this:
 *
 * ```
 * 8-bit   [ 0x7F ]
 *            |\\\_____________________
 *            | \\___________          \
 *            |  \____       \          \
 *            |       \       \          |
 *            |        |       |         |
 *            V        V       V         V
 * 32-bit  [ 0x7F ] [ 0x7F ] [ 0x7F ] [ 0x7F ]
 * ```
 * Expected 32-bit value is: 0x7FFFFFFF
 * Actual value from scaling: 0x7F7F7F7F
 * % difference is: (0x7FFFFFFF - 0x7F7F7F7F) / 0x7FFFFFFF = 0.39215684%
 * A precent difference well below 1% makes this a viable solution for most
 * applications.
 *
 * @tparam T - integral type to bring the resolution of U up to.
 * @tparam source_width - the bit resolution of the input value
 * @tparam U - integral type of the input value
 * @param p_value - the value to be scaled
 * @return constexpr T - p_value but with resolution scaled up to type T
 */
template<std::integral T, size_t source_width, std::integral U>
constexpr static T increase_bit_depth(U p_value)
{
  constexpr size_t output_bit_width = sizeof(T) * CHAR_BIT;

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
 * @brief A type based on bit width that contains a value
 *
 * @tparam T - the underlying type of the value
 * @tparam bit_width - the number of bits of the value
 *
 */
template<std::integral T, size_t bit_width>
struct bit_depth
{
  /**
   * @brief construct bit_depth object
   *
   * @param p_value - a integral value within the size of the bit_width. If the
   * object is outside the bounds of the bit width, it is clamped.
   *
   */
  constexpr bit_depth(T p_value)
    : value(p_value)
  {
    value = std::clamp(p_value,
                       bit_limits<bit_width, T>::min(),
                       bit_limits<bit_width, T>::max());
  }

  /// Value of the data at the bit width
  T value = 0;
};

/**
 * @brief A bit resolution erased representation of a precentage value using
 * only integral arithmetic
 *
 * @tparam T - the integral type to represent the percentage value (typically
 * uint32_t or int32_t)
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

  /**
   * @brief construct a full scale value with initial value zero.
   *
   */
  constexpr full_scale()
    : m_value(0)
  {}

  /**
   * @brief Construct a full_scale based on the bit_depth value
   *
   * @tparam U - type of the bit depth object
   * @tparam bit_width - the bit width of the object
   * @param p_value - the value at a specific bit resolution to scale up to a
   * full scale value
   * @return constexpr full_scale<T>& - reference to this object
   */
  template<typename U, size_t bit_width>
  constexpr full_scale<T>& operator=(bit_depth<U, bit_width> p_value)
  {
    static_assert(
      (std::is_signed_v<T> && std::is_signed_v<U>) ||
        (std::is_unsigned_v<T> && std::is_unsigned_v<U>),
      "The full scale arithemetic type and bit depth type must both be "
      "unsigned or signed. They cannot be mixed between each other.");

    m_value = increase_bit_depth<T, bit_width, U>(p_value.value);
    return *this;
  }

  /**
   * @brief Construct a full_scale based on the bit_depth value
   *
   * @tparam U - type of the bit depth object
   * @tparam bit_width - the bit width of the object
   * @param p_value - the bit depth object
   */
  template<typename U, size_t bit_width>
  constexpr full_scale(bit_depth<U, bit_width> p_value)
  {
    *this = p_value;
  }

  /**
   * @brief Get internal integral value
   *
   * @return T - full scale value
   */
  T value() { return m_value; }

private:
  T m_value = 0;
};

/**
 * @brief Scale an integral value by a full_scale<T> value.
 *
 * Returns a scaled down version of the input value. For example if the input is
 * 100 and the scale value represents a percentage of 50%, then performing the
 * following operation: `100 * full_scale_50_percent` is equivalent to
 * `100 * 0.5f`.
 *
 * @tparam T - underlying type of the full scale value
 * @tparam U - type of the integral value to be scaled
 * @param p_value - value to be scaled
 * @param p_scale - value scalar
 * @return auto - the scaled down result of p_value * p_scale.
 */
template<std::unsigned_integral T, std::unsigned_integral U>
auto operator*(U p_value, full_scale<T> p_scale)
{
  std::uintmax_t arith_container = p_value;
  arith_container = arith_container * p_scale.value();
  arith_container = arith_container / std::numeric_limits<T>::max();
  return static_cast<U>(arith_container);
}
}  // namespace embed
