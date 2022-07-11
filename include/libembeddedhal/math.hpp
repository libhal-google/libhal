#pragma once

#include <concepts>
#include <optional>

#include "error.hpp"

#define WIDE_INTEGER_DISABLE_IOSTREAM
#include "internal/third_party/uintwide_t.h"
#undef WIDE_INTEGER_DISABLE_IOSTREAM

namespace embed {
/**
 * @brief Perform multiply operation and return an error code
 * `std::errc::result_out_of_range` if the two values when multiplied would
 * overflow the containing value.
 *
 * @tparam T - integer arithmetic type
 * @param p_lhs - left hand side integer
 * @param p_rhs - right hand side integer
 * @return boost::leaf::result<T> - either the resultant or an error
 * `std::errc::result_out_of_range`
 */
template<typename T>
[[nodiscard]] boost::leaf::result<T> multiply_with_overflow_detection(T p_lhs,
                                                                      T p_rhs)
{
  if (p_lhs == 0 || p_rhs == 0) {
    return T{ 0 };
  }

  T result = p_lhs * p_rhs;

  if (p_lhs != result / p_rhs) {
    return boost::leaf::new_error(std::errc::result_out_of_range);
  }

  return result;
}

/**
 * @brief Generic absolute value function that works for integer types. This is
 * preferred over the C API for rounding numbers such as abs(), labs() and
 * llabs(). This function relieves the need in template code to check the type
 * of the integer and select the correct function to call.
 *
 * @param p_value - integer value to be made positive
 * @return constexpr auto - positive representation of the integer
 */
template<typename T>
[[nodiscard]] constexpr T absolute_value(T p_value) noexcept
{
  if (p_value >= 0) {
    return p_value;
  } else {
    return p_value * -1;
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
  bool num_sign = p_numerator > 0;
  bool den_sign = p_denominator > 0;

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
}  // namespace embed
