#pragma once

#include <concepts>

namespace embed {
/**
 * @brief Perform integer division and round the value up if the next decimal
 * place is greater than or equal to 0.5.
 *
 * @tparam T - integral type of the two operands
 * @param p_numerator - the value to be divided
 * @param p_denominator - the value to divide the numerator against
 * @return constexpr T - rounded quotent between numerator and denominator.
 * Returns 0 if the denominator is greater than the numerator.
 */
template<std::integral T>
constexpr T rounding_division(T p_numerator, T p_denominator)
{
  if (p_numerator < p_denominator) {
    return 0;
  }

  const T remainder = p_numerator % p_denominator;
  const T half_denominator = (p_denominator / 2);
  T quotient = p_numerator / p_denominator;
  // Round if remainder is greater than half of the denominator
  if (half_denominator != 0 && remainder >= half_denominator) {
    quotient++;
  }
  return quotient;
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
constexpr auto absolute_value(std::integral auto p_value)
{
  if (p_value >= 0) {
    return p_value;
  } else {
    return p_value * -1;
  }
}
}  // namespace embed
