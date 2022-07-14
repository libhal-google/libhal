/**
 * @file math.hpp
 * @brief Compile time generic math utility functions
 *
 */
#pragma once

#include "error.hpp"

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
    p_value = p_value * -1;
    return p_value;
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
}  // namespace embed
