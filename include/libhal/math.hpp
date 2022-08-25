/**
 * @file math.hpp
 * @brief Compile time generic math utility functions
 *
 */
#pragma once

#include <concepts>

#include "error.hpp"

namespace hal {
/**
 * @addtogroup utility
 * @{
 */

/**
 * @brief Perform multiply operation and return an error code
 * `std::errc::result_out_of_range` if the two values when multiplied would
 * overflow the containing value.
 *
 * @tparam T - integer arithmetic type
 * @param p_lhs - left hand side integer
 * @param p_rhs - right hand side integer
 * @return result<T> - either the resultant or an error
 * `std::errc::result_out_of_range`
 */
template<typename T>
[[nodiscard]] result<T> multiply(T p_lhs, T p_rhs) noexcept
{
  if (p_lhs == 0 || p_rhs == 0) {
    return T{ 0 };
  }

  T result = p_lhs * p_rhs;

  if (p_lhs != result / p_rhs) {
    return hal::new_error(std::errc::result_out_of_range);
  }

  return result;
}

/**
 * @brief Generic absolute value function that works for integer types.
 *
 * Preferred this over the C API for rounding numbers such as abs(), labs() and
 * llabs(). This function relieves the need in template code to check the type
 * of the integer and select the correct function to call.
 *
 * NOTE: If p_value is minimum negative number of type T then the resulting
 * return value will be the maximum positive number represented by T. For
 * example, INT32_MIN is 2147483648 where as INT32_MAX is 2147483647. The
 * absolute value of INT32_MIN is 1 greater than INT32_MAX. To prevent overflow,
 * passing INT32_MIN will simply return back INT32_MAX.
 *
 * @tparam T - integral type
 * @param p_value - integer value to be made positive
 * @return constexpr auto - positive representation of the integer
 */
template<typename T>
[[nodiscard]] constexpr T absolute_value(T p_value) noexcept
{
  if constexpr (std::is_unsigned_v<T>) {
    return p_value;
  } else {
    if (p_value >= 0) {
      return p_value;
    }
    if (p_value == std::numeric_limits<T>::min()) {
      return std::numeric_limits<T>::max();
    }
    return -p_value;
  }
}

/**
 * @brief Perform integer division and round the value up if the next decimal
 * place is greater than or equal to 0.5.
 *
 * @tparam T - integral type of the two operands
 * @param p_numerator - the value to be divided
 * @param p_denominator - the value to divide the numerator against
 * @return constexpr T - rounded quotient between numerator and denominator.
 * Returns 0 if the denominator is greater than the numerator.
 */
template<typename T>
[[nodiscard]] constexpr T rounding_division(T p_numerator,
                                            T p_denominator) noexcept
{
  bool num_sign = p_numerator >= 0;
  bool den_sign = p_denominator >= 0;

  auto numerator = absolute_value(p_numerator);
  auto denominator = absolute_value(p_denominator);

  if (denominator > numerator || denominator == 0) {
    return T{ 0 };
  }

  const T remainder = numerator % denominator;
  const T half_denominator = (denominator / 2);
  T quotient = numerator / denominator;
  // Round if remainder is greater than half of the denominator
  if (half_denominator != 0 && remainder >= half_denominator) {
    quotient++;
  }

  if (num_sign != den_sign) {
    return -quotient;
  }

  return quotient;
}

/**
 * @brief Calculates the distance between two values (L1 Norm or Manhattan
 * distance), the absolute value of their difference.
 *
 * @tparam T - integral type of the two values
 * @param p_left - the first point of the distance calculation
 * @param p_right - the second point of the distance calculation
 * @returns constexpr T - absolute value of the difference between the two
 * points.
 */
template<typename T>
[[nodiscard]] constexpr T distance(T p_left, T p_right) noexcept
{
  if (p_right > p_left) {
    return p_right - p_left;
  } else {
    return p_left - p_right;
  }
}

/**
 * @brief Calculates the distance between two values (L1 Norm or Manhattan
 * distance), the absolute value of their difference.
 *
 * NOTE: Values cannot exceed int32_t.
 *
 * @tparam T - integral type of the two values
 * @param p_left - the first point of the distance calculation
 * @param p_right - the second point of the distance calculation
 * @returns constexpr T - absolute value of the difference between the two
 * points.
 */
template<std::integral T>
[[nodiscard]] constexpr std::make_unsigned_t<T> distance(T p_left,
                                                         T p_right) noexcept
{
  static_assert(sizeof(T) <= sizeof(int32_t));

  using unsigned_t = std::make_unsigned_t<T>;
  // Put left and right values into 64-bit containers to prevent overflow
  int64_t left{ p_left };
  int64_t right{ p_right };

  if (right > left) {
    // Subtraction operation on right to left in this order can never overflow
    // because the maximum resultant of left and right being INT32_MAX and
    // INT32_MIN, will equal UINT32_MAX which can be stored within an int64_t
    // value.
    int64_t difference = right - left;
    // Casting this value to the unsigned variant will always fit as the
    // distance between any 32-bit signed numbers can always fit in a 32-bit
    // unsigned number.
    return static_cast<unsigned_t>(difference);
  } else {
    // Same logic as the if statement block above.
    int64_t difference = left - right;
    return static_cast<unsigned_t>(difference);
  }
}
/**
 * @brief Determines if two values are equal within a relative error.
 *
 * @param p_value1 - First value to compare.
 * @param p_value2 - Second value to compare.
 * @param p_epsilon - Error margin that the difference is compared to.
 * @return true - difference is less than epsilon
 * @return false - difference is more than epsilon
 */
constexpr static bool equals(std::floating_point auto p_value1,
                             std::floating_point auto p_value2,
                             float p_epsilon = 1e-9f)
{
  if (p_value1 == p_value2) {
    return true;
  }
  auto value1_abs = std::abs(p_value1);
  auto value2_abs = std::abs(p_value2);
  auto diff = std::abs(p_value1 - p_value2);
  auto absolute_values_sum = value1_abs + value2_abs;

  using float_t = decltype(absolute_values_sum);

  if (p_value1 == 0 || p_value2 == 0 ||
      (absolute_values_sum < std::numeric_limits<float_t>::min())) {
    return diff < (p_epsilon * std::numeric_limits<float_t>::min());
  } else {
    auto relative_error =
      diff / std::min(absolute_values_sum, std::numeric_limits<float_t>::max());
    return relative_error < p_epsilon;
  }
}
/** @} */
}  // namespace hal
