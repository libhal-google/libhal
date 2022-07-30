#pragma once

#include <algorithm>

#include "math.hpp"
#include "percent.hpp"

namespace hal {
/**
 * @addtogroup utility
 * @{
 */

/**
 * @brief Structure that contains a range from x to y or y to x.
 *
 * @tparam T - integral, percent or iterator types
 */
template<typename T>
struct map_range
{
  /// A point in the range
  T x{};
  /// The opposite point in the range
  T y{};

  /**
   * @return T - the minimum value in the range
   */
  T min()
  {
    return std::min(x, y);
  }
  /**
   * @return T - the maximum value in the range
   */
  T max()
  {
    return std::max(x, y);
  }
  /**
   * @return auto - the distance between x & y
   */
  auto distance()
  {
    return hal::distance(x, y);
  }
};

/**
 * @brief Map an target value [x] from an input range [a,b] to an output range
 * [c,d].
 *
 * Another term for this is an affine transformation which follows this
 * equation:
 *
 *                       / d - c \
 *        y = (x - a) * | --------| + c
 *                       \ b - a /
 *
 * For example:
 *
 *    let x = 5
 *    let input_range = [0,10]
 *    let output_range = [100, 200]
 *    The result will be 150.
 *
 * @tparam T - integral type equal to or below 32-bits
 * @param p_target - target value within p_input_range to be mapped to the
 * output range.
 * @param p_input_range - the input range of p_target
 * @param p_output_range - the output range to map p_target to
 * @return constexpr result<T> - value mapped from input range to
 * the output range.
 * @throws std::errc::invalid_argument if input_range has a distance of zero.
 */
template<typename T>
[[nodiscard]] constexpr result<T> map(T p_target,
                                      map_range<T> p_input_range,
                                      map_range<T> p_output_range) noexcept
{
  static_assert(sizeof(T) <= sizeof(std::uint32_t));

  // Use x & y directly here because distance will do a range check itself
  auto output_distance = p_output_range.distance();
  auto input_distance = p_input_range.distance();

  // If the output range's distance is 0 it means that x & y are both the same
  // value. Thus all target values map to the same value in the output and thus
  // the result is simply x or y.
  if (std::cmp_equal(output_distance, 0)) {
    return p_output_range.x;
  }
  // If the input range is 0, then the input range is a single number. Mapping a
  // 1-dimensional range to a 2-D range is not defined and thus the inputs are
  // not valid.
  //
  // Throw error std::errc::invalid_argument.
  if (std::cmp_equal(input_distance, 0)) {
    return boost::leaf::new_error(std::errc::invalid_argument);
  }

  // Bounds check p_target and if it is is outside of the input range then
  // return clamped the return value to the bounds of the output range.
  if (p_target >= p_input_range.max()) {
    return p_output_range.max();
  } else if (p_target <= p_input_range.min()) {
    return p_output_range.min();
  }

  // Verify at compile time that the types for "output_distance" &
  // "input_distance" are both unsigned 32 bit integers
  static_assert(std::is_same_v<decltype(output_distance), std::uint32_t>);
  static_assert(std::is_same_v<decltype(input_distance), std::uint32_t>);

  // Performing integer division & modulus which will floor the result and
  // provide the whole number solution. This operation is safe from "divide by
  // zero" errors as because input_distance is checked for being zero above.
  auto slope_quotient = output_distance / input_distance;
  auto slope_remainder = output_distance % input_distance;

  // Operation is well defined for integers 32-bit and below
  auto decimal = hal::percent::from_ratio(slope_remainder, input_distance);

  // p_target has already been bounds checked and shown to be within the bounds
  // of p_input_range, thus this subtracts operation will never yield an
  // underflow or overflow.
  auto align_target_to_zero = p_target - p_input_range.min();

  // This operation will never overflow because the ratio between two ranges
  // confined to the integral type T times the zero aligned target value will
  // never exceed the bounds of T.
  auto quotient_scaled_value = slope_quotient * align_target_to_zero;
  // Multiplication between hal::percent & a 32-bit number is always a safe
  // operation.
  auto remainder_scaled_value = decimal * align_target_to_zero;

  // Summing the two values will produce a value within the bounds of the output
  // range's distance.
  auto scaled_value = quotient_scaled_value + remainder_scaled_value;
  // Adding the output range's minimum value will push the scaled value directly
  // to where it should be mapped in the output range.
  auto scaled_value_with_offset = p_output_range.min() + scaled_value;

  return scaled_value_with_offset;
}
/** @} */
}  // namespace hal
