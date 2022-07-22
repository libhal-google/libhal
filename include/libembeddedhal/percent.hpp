#pragma once

#include <algorithm>
#include <charconv>
#include <climits>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

#include "math.hpp"
#include "to_array.hpp"

namespace embed {
/**
 * @addtogroup utility
 * @{
 */

/**
 * @brief Upscale an integer value of arbitrary bit resolution to a new fixed
 * with integer value.
 *
 * The purpose of bit scaling a value is to take a value of lower bit
 * resolution, scale it up but keep the percentage relative to the bits
 * resolution. This is useful for bit resolution erasure as well as scaling up
 * pixel depth in images.
 *
 * For example, lets take an 8-bit value of 127 (or 0x7F). This value
 * is 50% of an 8-bit number. 50% of an 32-bit would be 2147483647 or 0x7FFFFFFF
 * which is half of 2^32. A perfect up scale would take 0x7F and generate
 * 0x7FFFFFFF in this case. Doing so is quite costly and requires multiplication
 * and division operations which are slower operation.
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
 * A percent difference well below 1% makes this a viable solution for most
 * applications.
 *
 * @tparam T - integral type to bring the resolution of U up to.
 * @tparam SourceWidth - the bit resolution of the input value
 * @tparam U - integral type of the input value
 * @param p_value - the value to be scaled
 * @return constexpr T - p_value but with resolution scaled up to type T
 */
template<std::integral T, size_t SourceWidth, std::integral U>
[[nodiscard]] constexpr T upscale_integer(U p_value) noexcept
{
  constexpr size_t output_bit_width = sizeof(T) * CHAR_BIT;

  static_assert(output_bit_width >= SourceWidth,
                "The destination bit width must be equal to or greater than "
                "the type of the value to be scaled up.");

  static_assert(SourceWidth >= 2, "Bit Width must be greater than ");

  constexpr size_t final_source_width =
    (std::is_signed_v<U>) ? SourceWidth - 1 : SourceWidth;
  constexpr size_t final_width =
    (std::is_signed_v<T>) ? output_bit_width - 1 : output_bit_width;

  // Calculate the difference between the destination and the source width
  constexpr size_t width_difference = final_width - final_source_width;

  T result = static_cast<T>(p_value << width_difference);

  if (p_value > 0) {
    for (size_t i = final_source_width; i < final_width;
         i += final_source_width) {
      result |= (result >> i);
    }
  }

  return result;
}

/**
 * @brief A integer representation of a percentage.
 *
 * Primary usage is representing numbers with a arbitrary bit-resolution and up
 * scaling them to higher bit resolutions while retaining their proportionality.
 *
 * For example, if a device has an 10-bit unsigned ADC, its range of possible
 * output is 0 to 1024. If a device driver requires an ADC, that driver will
 * need to know the ADC value and the bit resolution in order to determine
 * where the decimal number fits within the range of the ADC. percent
 * eliminates this by scaling up all numbers to uint32_t or int32_t and keeping
 * the proportion within the range the same. So 512 out of 1024 is 50% and this
 * class will preserve that 50% value proportional value but within a 32-bit
 * integer.
 *
 */
class percent
{
public:
  /// The representation of the percentage will be contained within this type
  using int_t = std::int32_t;
  /// The overflow type must be 2x the size of int_t in order to perform
  /// multiplication against two int_t value and not lose any data.
  using overflow_t = std::int64_t;
  static_assert(sizeof(overflow_t) >= sizeof(int_t),
                "Overflow integer type must be equal to or greater ");

  /**
   * @brief Get the 100% value in its raw representation
   *
   * @return constexpr overflow_t - maximum limit of int_t
   */
  [[nodiscard]] static constexpr overflow_t raw_max() noexcept
  {
    return std::numeric_limits<int_t>::max();
  }

  /**
   * @brief Get the -100% value in its raw representation
   *
   * @return constexpr overflow_t - minimum limit of int_t
   */
  [[nodiscard]] static constexpr overflow_t raw_min() noexcept
  {
    return std::numeric_limits<int_t>::min() + overflow_t{ 1 };
  }

  /**
   * @brief Get the 0% value in its raw representation
   *
   * @return constexpr overflow_t - 0 value for int_t
   */
  [[nodiscard]] static constexpr overflow_t raw_zero() noexcept
  {
    return int_t{ 0 };
  }
  /**
   * @brief Get +100% percentage
   *
   * @return constexpr percent - maximum positive percentage
   */
  [[nodiscard]] static constexpr percent max() noexcept
  {
    return percent(raw_max());
  }

  /**
   * @brief Get -100% percentage
   *
   * @return constexpr percent - maximum negative percentage
   */
  [[nodiscard]] static constexpr percent min() noexcept
  {
    return percent(raw_min());
  }

  /**
   * @brief Get 0% percentage
   *
   * @return constexpr percent - zero percentage
   */
  [[nodiscard]] static constexpr percent zero() noexcept
  {
    return percent{};
  }

  /**
   * @brief Convert a fixed width integer value into a percentage based on its
   * distance to the end of its bit width.
   *
   * @tparam BitWidth - The bit width of the input value
   * @tparam T - integral type of input value
   * @param p_value - the value of the number
   * @return constexpr percent - the percent type based on the input value's
   * distance to the end of the bit width.
   */
  template<size_t BitWidth, std::integral T>
  [[nodiscard]] static constexpr percent convert(T p_value) noexcept
  {
    const int_t up_scaled_value = upscale_integer<int_t, BitWidth, T>(p_value);
    return percent(up_scaled_value);
  }

  /**
   * @brief Generate a percentage based on input relative to the maximum
   * possible value.
   *
   * The output will be clamped to -100% and 100% if the input value is above or
   * below the constraints of the maximum.
   *
   * Examples:
   *
   *    To generate 50%, use embed::percent::from_ratio(1, 2);
   *    To generate 50%, use embed::percent::from_ratio(50, 100);
   *    To generate 50%, use embed::percent::from_ratio(250, 500);
   *    To generate 20%, use embed::percent::from_ratio(20, 100);
   *    To generate 14%, use embed::percent::from_ratio(35, 250);
   *
   * @tparam T - integral types for the numerator and denominator
   * @param p_progress - the number in which a percentage will be generated
   * relative to how close the value is to the p_maximum value. This value can
   * be considered the numerator of a rational number where the denominator is
   * p_maximum. The result will be clamped if the input is above the maximum
   * value.
   * @param p_maximum - the absolute maximum value of the ratio and the
   * indicator of 100% and -100% progress. Can consider this value as a
   * denominator of a ration number.
   * @return constexpr percent
   */
  template<std::integral T>
  [[nodiscard]] static constexpr percent from_ratio(T p_progress,
                                                    T p_maximum) noexcept
  {
    static_assert(sizeof(T) <= sizeof(std::uint32_t),
                  "T must not exceed 32 bits in width.");
    overflow_t result = p_progress;
    // Multiplication between result, which must be size 32-bits or lower times
    // raw_max() which is defined as 32-bit can be held in the overflow type
    // which is int64_t, thus no overflow can occur here.
    result = (result * raw_max());
    // If p_progress is less than p_maximum then the result value will fit
    // within a 32-bit integer.
    result = rounding_division(result, overflow_t(p_maximum));
    // If p_progress was greater than p_maximum, then the end result will be
    // greater than raw_max() or less than raw_min().
    // This stage will clamp the result to between the 32-bit.
    result = std::clamp(result, raw_min(), raw_max());

    return percent(static_cast<int_t>(result));
  }

  [[nodiscard]] static constexpr percent from_raw(int_t p_raw_value)
  {
    return percent(p_raw_value);
  }

  /**
   * @brief Construct 0% percent object
   *
   */
  constexpr percent() noexcept
    : m_value(0)
  {
  }

  /**
   * @brief Construct a percent based on a floating point percentage value.
   *
   * @param p_ratio - floating point ratio value. For signed numbers this is
   * clamped between 0.0 and 1.0. For signed numbers it is clamped between -1.0
   * to 1.0.
   */
  constexpr percent(std::floating_point auto p_ratio) noexcept
  {
    *this = p_ratio;
  }

  /**
   * @brief Assignment operator for a percent object based on a floating point
   * value.
   *
   * @param p_ratio - floating point ratio value. For signed numbers this is
   * clamped between 0.0 and 1.0. For signed numbers it is clamped between -1.0
   * to 1.0.
   * @return constexpr percent& - integer percent object based on the floating
   * point percent value.
   */
  constexpr percent& operator=(std::floating_point auto p_ratio) noexcept
  {
    using float_t = decltype(p_ratio);

    constexpr float_t max = 1.0;
    constexpr float_t min = -1.0;
    p_ratio = std::clamp(p_ratio, min, max);

    if constexpr (std::is_same_v<float_t, float>) {
      m_value = static_cast<int_t>(std::roundf(p_ratio * raw_max()));
    } else if constexpr (std::is_same_v<float_t, double>) {
      m_value = static_cast<int_t>(std::round(p_ratio * raw_max()));
    } else if constexpr (std::is_same_v<float_t, long double>) {
      m_value = static_cast<int_t>(std::roundl(p_ratio * raw_max()));
    }

    return *this;
  }

  /**
   * @brief Convert percent to a floating point representation
   *
   * @tparam T - floating point type
   * @return constexpr T - float representation of the percentage between 0.0f
   * and 1.0f.
   */
  template<std::floating_point T>
  [[nodiscard]] constexpr T to() const noexcept
  {
    constexpr T maximum = static_cast<T>(raw_max());
    return static_cast<T>(m_value) / maximum;
  }

  /**
   * @brief explicit cast to float.
   *
   * @return float - float representation of the percentage between 0.0f
   * and 1.0f.
   */
  [[nodiscard]] explicit operator float() const noexcept
  {
    return to<float>();
  }

  /**
   * @brief explicit cast to double.
   *
   * @return double - double representation of the percentage between 0.0
   * and 1.0
   */
  [[nodiscard]] explicit operator double() const noexcept
  {
    return to<double>();
  }

  /**
   * @brief Scale an integral value by a percent value.
   *
   * Returns a scaled down version of the input value. For example if the input
   * is 100 and the scale value represents a percentage of 50%, then performing
   * the following operation: `100 * percent_50_percent` is equivalent to `100 *
   * 0.5f`.
   *
   * @tparam T - type of the integral value to be scaled
   * @param p_value - value to be scaled
   * @param p_scale - value scalar
   * @return auto - the scaled down result of p_value * p_scale.
   */
  template<std::integral T>
  [[nodiscard]] friend constexpr auto operator*(T p_value,
                                                percent p_scale) noexcept
  {
    static_assert(sizeof(T) <= sizeof(std::int32_t),
                  "T must not exceed 32 bits in width.");
    overflow_t arith_container = p_value;
    arith_container = arith_container * p_scale.raw_value();
    arith_container = rounding_division(arith_container, raw_max());
    return static_cast<T>(arith_container);
  }

  /**
   * @brief Same as `operator*(U p_value, percent p_scale)`
   *
   * @tparam T - see other operator*
   * @param p_scale - see other operator*
   * @param p_value - see other operator*
   * @return constexpr auto - see other operator*
   */
  template<std::integral T>
  [[nodiscard]] friend constexpr auto operator*(percent p_scale,
                                                T p_value) noexcept
  {
    return p_value * p_scale;
  }

  percent operator-() const
  {
    percent negated_percent(-m_value);
    return negated_percent;
  }

  /**
   * @brief Get the percent after it is scaled to a range. The output percent
   * is bound within the given range. If the percent is greater than or equal
   * to the maximum bounds of the range, then 100 percent is returned.
   * Likewise, if the percent is less than or equal to the minimum bounds of
   * the range, then 0 percent is returned.
   *
   * Examples:
   * If the range is [0, 100] and the percent is 50, output should be 50%
   * If the range is [-100, 100] and the percent is -50, output should be 25%
   * If the range is [-100, 100] and the percent is 0, output should be 50%
   * If the range is [-100, 100] and the percent is 50, output should be 75%
   * If the range is [-10, 100] and the percent is 45, output should be 50%
   *
   * @param p_value1 - first value of the range
   * @param p_value2 - second value of the range
   * @return percent - percent after it has been scaled
   */
  [[nodiscard]] constexpr percent scale(percent p_value1, percent p_value2)
  {
    overflow_t min =
      static_cast<overflow_t>(std::min(p_value1, p_value2).m_value);
    overflow_t max =
      static_cast<overflow_t>(std::max(p_value1, p_value2).m_value);

    if (m_value >= max) {
      return from_ratio(1, 1);
    }
    if (m_value <= min) {
      return from_ratio(0, 1);
    }

    overflow_t numerator = static_cast<overflow_t>(m_value) - min;
    overflow_t denominator = max - min;
    overflow_t scaled_value =
      rounding_division((numerator * raw_max()), denominator);
    scaled_value = std::clamp(scaled_value, raw_min(), raw_max());
    return percent(static_cast<int_t>(scaled_value));
  }

  /**
   * @brief Get the value of the percent after it is scaled to a range. The
   * output value is bound within the given range. If the percent is greater
   * than or equal to 100 percent, then the maximum value of the range is
   * returned. Likewise, if the value is less than or equal to 0 percent, then
   * the minimum value of the range is returned.
   *
   * Examples:
   * If the range is [0, 100] and the percent is 25, output should be 25
   * If the range is [0, 100] and the percent is 75, output should be 75
   * If the range is [-100, 0] and the percent is 25, output should be -75
   * If the range is [-100, 0] and the percent is 75, output should be -25
   * If the range is [-100, 100] and the percent is 25, output should be -50
   * If the range is [-100, 100] and the percent is 75, output should be 50
   * If the range is [-100, 100] and the percent is 0, output should be -100
   * If the range is [-100, 100] and the percent is 100, output should be 100
   *
   * @tparam T - types for range and return value
   * @param p_value1 - first value of the range
   * @param p_value2 - second value of the range
   * @return T - value after it has been scaled
   */
  template<std::integral T>
  [[nodiscard]] T scale(T p_value1, T p_value2)
  {
    T min = std::min(p_value1, p_value2);
    T max = std::max(p_value1, p_value2);

    if (m_value == raw_max()) {
      return max;
    } else if (m_value <= raw_zero()) {
      return min;
    }

    T scaled_max = (*this) * max;
    T scaled_min = (*this) * min;
    T scaled_output = min + scaled_max - scaled_min;
    return scaled_output;
  }

  /**
   * @brief Get raw integral value
   *
   * @return T - percent value
   */
  [[nodiscard]] constexpr auto raw_value() const noexcept
  {
    return m_value;
  }

  /**
   * @brief Default operators for <, <=, >, >= and ==
   *
   * @return auto - result of the comparison
   */
  [[nodiscard]] constexpr auto operator<=>(const percent&) const noexcept =
    default;

  /**
   * @brief Comparison operator
   *
   * Percentages are considered equal if their raw values are equal plus or
   * minus 2 values. This is to account for slight rounding errors between
   * math operations. A distance of value 2 represents an error of
   * 0.00000004657%.
   *
   * @param p_other - the other percent to compare to this one
   * @return auto - result of the comparison
   */
  [[nodiscard]] constexpr auto operator==(const percent& p_other) const noexcept
  {
    auto delta = distance(raw_value(), p_other.raw_value());
    if (delta <= 2) {
      return true;
    }
    return false;
  }

private:
  constexpr percent(int_t p_value) noexcept
    : m_value(p_value)
  {
  }

  int_t m_value = 0;
};

/**
 * @brief convert this percentage value into a string from -1.0 to +1.0
 *
 * Strings are computed using integer arithmetic only.
 *
 * The format of the string will follow these rules:
 *   - Will always have a leading + or - sign
 *   - Will always be 13 characters where the last character is the '\0'
 *   - Will start with either a '1' or a '0' character
 *
 * Example string:
 *
 *   - +1.000000000
 *   - +0.250000000
 *   - +0.125000000
 *   - -0.333333333
 *   - -0.111111111
 *   - -0.666666667
 *
 * @return auto - string representation of the percent.
 */
[[nodiscard]] constexpr auto to_string(const percent& p_percent) noexcept
{
  using int_t = percent::int_t;

  constexpr int_t fixed_percent_scalar = 1000000000;
  // Based on the number of characters needed to hold "fixed_percent_scalar"
  // as well as a '.' and a '-' sign
  constexpr size_t scalar_length = 12;

  if (p_percent == percent::max()) {
    return to_array<scalar_length>("+1.000000000");
  } else if (p_percent == percent::min()) {
    return to_array<scalar_length>("-1.000000000");
  }

  percent positive_percent = p_percent;
  // If negative absolute, make the percent positive
  bool negative = p_percent.raw_value() < 0;
  if (negative) {
    positive_percent = -p_percent;
  }

  // +1 for a '\0' at the end
  std::array<char, scalar_length + 1> buffer{ '\0' };
  // +2 for a '.' and a '\0' character at the end
  std::array<char, scalar_length + 1> percent_string{ '\0' };

  // Scale "fixed_percent_scalar" by the absolute_percent value
  int_t decimal_percent = positive_percent * fixed_percent_scalar;

  // Convert to characters
  auto char_conversion_result =
    std::to_chars(buffer.begin(), buffer.end(), decimal_percent);

  size_t string_length = char_conversion_result.ptr - buffer.data();
  size_t leading_zeros = scalar_length - string_length;

  // Add the leading +/- sign
  percent_string[0] = (negative) ? '-' : '+';

  // Add the '0' and '.'. We know that the value must be below 1.0 and above
  // -1.0 because if the values were 1.0 or -1.0 then we would have returned
  // them at the start of the function.
  percent_string[1] = '0';
  percent_string[2] = '.';

  // Add any leading zeros into the percent string array
  for (size_t i = 3; i < leading_zeros; i++) {
    percent_string[i] = '0';
  }
  // Now copy the contents of the character buffer into the percent string
  // offset by the number of leading zeros.
  std::copy_n(
    buffer.begin(), string_length, percent_string.begin() + leading_zeros);

  return percent_string;
}

/** @} */
}  // namespace embed
