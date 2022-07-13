#pragma once

#include <algorithm>
#include <chrono>
#include <cinttypes>
#include <cstdint>
#include <limits>
#include <ratio>

#include "math.hpp"
#include "percent.hpp"
#include "units.hpp"

namespace embed {
/**
 * @brief Structure containing cycle count for the high and low side of a signal
 * duty cycle.
 *
 */
struct duty_cycle
{
  /// Number of cycles the signal will stay in the HIGH voltage state
  std::uint32_t high = 0;
  /// Number of cycles the signal will stay in the LOW voltage state
  std::uint32_t low = 0;

  /**
   * @brief == operator
   *
   * @param p_cycle - other duty scale to compare to
   * @return constexpr auto - true if the duty cycles have the exact same
   * values.
   */
  [[nodiscard]] constexpr auto operator==(
    const duty_cycle& p_cycle) const noexcept
  {
    return (high == p_cycle.high) && (low == p_cycle.low);
  }

  /**
   * @brief Conversion from duty cycle to percentage
   *
   * @return constexpr percentage - Percentage of high cycles to the total cycle
   * count.
   */
  [[nodiscard]] explicit constexpr operator percent() const noexcept
  {
    std::uint64_t total_cycles = high + low;
    return percent::from_ratio(static_cast<std::uint64_t>(high), total_cycles);
  }
};

/**
 * @brief Represents the frequency of a signal. It consists of a single integer
 * 64-bit number that presents the integer representation of a signal frequency.
 *
 */
class frequency
{
public:
  /**
   * @brief Generate a duty_cycle object based on the percent value and
   * the input count value. The count value is split based on the ratio within
   * percent
   *
   * @param p_cycles - the number of cycles to be split into a duty cycle
   * @param p_percent - the target duty cycle percentage
   * @return constexpr duty_cycle - the duty cycle cycle counts
   */
  [[nodiscard]] static constexpr duty_cycle calculate_duty_cycle(
    std::uint32_t p_cycles,
    percent p_percent) noexcept
  {
    // Scale down value based on the integer percentage value in percent
    std::uint32_t high = p_cycles * p_percent;
    // p_cycles will always be larger than or equal to high
    std::uint32_t low = p_cycles - high;

    return duty_cycle{
      .high = high,
      .low = low,
    };
  }

  /**
   * @brief construct the frequency object
   *
   * @param p_value - frequency of the object
   */
  explicit constexpr frequency(std::uint32_t p_value) noexcept
    : m_cycles_per_second(p_value)
  {
  }

  /**
   * @brief Get the frequency as an integer
   *
   * @return constexpr auto - frequency value as an integer
   */
  [[nodiscard]] constexpr auto cycles_per_second() const noexcept
  {
    return m_cycles_per_second;
  }

  /**
   * @brief Calculate the frequency divide required to generate the output
   * frequency provided.
   *
   * @param p_target - the target output frequency
   * @return constexpr std::uint32_t - the divide, when applied to this
   * frequency, will achieve the p_target frequency. A value of is an error and
   * 0 indicates that the output frequency is greater than this frequency and
   * there does not exist an integer divide that can produce the output
   * frequency.
   */
  [[nodiscard]] constexpr std::uint32_t divide(
    frequency p_target) const noexcept
  {
    return rounding_division(cycles_per_second(), p_target.cycles_per_second());
  }

  /**
   * @brief Calculate the number of cycles of this frequency within the time
   * duration. This function is meant for timers to determine how many count
   * cycles are needed to reach a particular time duration at this frequency.
   *
   * @param p_duration - the amount of time to convert to cycles
   * @return std::int64_t - number of cycles
   */
  [[nodiscard]] std::int64_t cycles_per(
    embed::time_duration p_duration) const noexcept
  {
    // Full Equation:
    // =========================================================================
    //
    //                              / ratio_num \_
    //   frequency_hz * |period| * | ----------- |  = cycles
    //                              \ ratio_den /
    //
    // std::chrono::nanoseconds::period::num == 1
    // std::chrono::nanoseconds::period::den == 1,000,000

    constexpr std::int64_t numerator = decltype(p_duration)::period::num;
    constexpr std::int64_t denominator = decltype(p_duration)::period::den;
    // Storing 32-bit value in a std::uint64_t for later computation, no
    // truncation possible.
    const std::int64_t duration = absolute_value(p_duration.count());
    // Duration contains at most a 64-bit number, cycles_per_second() is a
    // 32-bit number, and numerator is always the value 1.
    //
    // To contain the maximum resultant possible requires storage within an
    // integer of size 64-bits, and thus will fit within a std::uint64_t
    // variable, no overflow checks required.
    const std::int64_t count = duration * cycles_per_second() * numerator;
    const std::int64_t cycle_count = rounding_division(count, denominator);

    return cycle_count;
  }

  /**
   * @brief Calculates and returns the wavelength of the frequency in
   * seconds.
   *
   * @tparam Period - desired period (defaults to std::femto for femtoseconds).
   * @return std::chrono::duration<int64_t, Period> - time based wavelength of
   * the frequency.
   */
  template<typename Period = std::femto>
  std::chrono::duration<int64_t, Period> wavelength() const
  {
    // Full Equation (based on the equation in cycles_per()):
    // =========================================================================
    //
    //                /  cycle_count * ratio_den \_
    //   |period| =  | ---------------------------|
    //                \ ratio_num * frequency_hz /
    //
    // let cycle_count = 1
    // --> Wavelength is the length of a single cycle
    //
    // let ratio_num = 1
    // --> Smallest frequency can only be as small as 1Hz. No wavelengths with
    //     numerators

    static_assert(Period::num == 1,
                  "Period::num of 1 are allowed for this function.");
    static_assert(Period::den <= 1000000000000000000,
                  "Period::den cannot exceed 1000000000000000000.");

    std::uint64_t numerator = Period::den;
    std::uint64_t denominator = cycles_per_second();
    std::uint64_t duration = rounding_division(numerator, denominator);
    return std::chrono::duration<int64_t, Period>(duration);
  }

  /**
   * @brief Calculate time duration based on the frequency and a number of
   * cycles.
   *
   * @param p_cycles - number of cycles within the time duration
   * @return std::chrono::nanoseconds - time duration based on this frequency
   * and the number of cycles
   */
  [[nodiscard]] std::chrono::nanoseconds duration_from_cycles(
    std::int32_t p_cycles) const noexcept
  {
    // Full Equation (based on the equation in cycles_per()):
    // =========================================================================
    //
    //                /    cycles * ratio_den    \_
    //   |period| =  | ---------------------------|
    //                \ frequency_hz * ratio_num /
    //
    constexpr auto time_duration_den = std::chrono::nanoseconds::period::den;
    constexpr auto time_duration_num = std::chrono::nanoseconds::period::num;
    constexpr auto scale_den = std::int64_t{ time_duration_den };
    constexpr auto scale_num = std::int64_t{ time_duration_num };

    std::int64_t numerator = std::int64_t{ p_cycles } * scale_den;
    std::int64_t denominator = cycles_per_second() * scale_num;
    std::int64_t nanoseconds = rounding_division(numerator, denominator);

    return std::chrono::nanoseconds(nanoseconds);
  }

  /**
   * @brief Calculate a duty cycle based on a time duration, the percent
   * and this driving frequency. Typically used for PWM or clock lines with
   * controllable duty cycles for serial communication.
   *
   * @param p_duration - target time duration to reach
   * @param p_percent - ratio of the duty cycle high time
   * @return constexpr duty_cycle
   */
  [[nodiscard]] boost::leaf::result<duty_cycle> calculate_duty_cycle(
    embed::time_duration p_duration,
    percent p_percent) const noexcept
  {
    std::uint64_t cycles = cycles_per(p_duration);
    if (cycles > std::numeric_limits<std::uint32_t>::max()) {
      return boost::leaf::new_error(std::errc::value_too_large);
    }
    return calculate_duty_cycle(static_cast<uint32_t>(cycles), p_percent);
  }

  /**
   * @brief Calculate a duty cycle based on a target, the percent
   * and this driving frequency. Typically used for PWM or clock lines with
   * controllable duty cycles for serial communication.
   *
   * @tparam T - containing type of the percent
   * @param p_target - target frequency to reach
   * @param p_percent - ratio of the duty cycle high time
   * @return constexpr duty_cycle
   */
  [[nodiscard]] constexpr duty_cycle calculate_duty_cycle(
    frequency p_target,
    percent p_percent) const noexcept
  {
    return calculate_duty_cycle(divide(p_target), p_percent);
  }

  /**
   * @brief Default operators for <, <=, >, >= and ==
   *
   * @return auto - result of the comparison
   */
  [[nodiscard]] constexpr auto operator<=>(const frequency&) const noexcept =
    default;

  /**
   * @brief Scale down a frequency by an integer factor
   *
   * @param p_lhs - the frequency to be scaled
   * @param p_rhs - the integer value to scale the frequency by
   * @return constexpr frequency
   */
  [[nodiscard]] constexpr friend frequency operator/(
    frequency p_lhs,
    std::uint32_t p_rhs) noexcept
  {
    return frequency{ rounding_division(p_lhs.cycles_per_second(), p_rhs) };
  }

  /**
   * @brief Scale down a frequency by an integer factor
   *
   * @param p_input - the input frequency to be divided down to the target
   * frequency with an integer divide
   * @param p_target - the target frequency to reach via an integer divide
   * @return constexpr std::uint32_t - frequency divide value representing
   * the number of cycles in the input that constitute one cycle in the target
   * frequency.
   */
  [[nodiscard]] constexpr friend std::uint32_t operator/(
    frequency p_input,
    frequency p_target) noexcept
  {
    return p_input.divide(p_target);
  }

  /**
   * @brief Scale up a frequency by an integer factor
   *
   * @tparam Integer - type of unsigned integer
   * @param p_scalar - the value to scale the frequency up to
   * @return constexpr frequency
   */
  template<std::unsigned_integral Integer>
  [[nodiscard]] boost::leaf::result<frequency> scale(
    Integer p_scalar) const noexcept
  {
    return frequency{ BOOST_LEAF_CHECK(
      multiply_with_overflow_detection(cycles_per_second(), p_scalar)) };
  }

private:
  std::uint32_t m_cycles_per_second = 1'000'000;
};

/// Default clock rate for serial communication protocols
constexpr frequency default_clock_rate = frequency(100'000);

namespace literals {
/**
 * @brief user defined literals for making frequencies: 1337_Hz
 *
 * Example range: 1337_Hz == 1,337 Hz
 *
 * @param p_value - frequency in hertz
 * @return consteval frequency - frequency in hertz
 */
[[nodiscard]] consteval frequency operator""_Hz(
  unsigned long long p_value) noexcept
{
  return frequency{ static_cast<std::uint32_t>(p_value) };
}

/**
 * @brief user defined literals for making frequencies in the kilohertz.
 *
 * Example range: 20_kHz == 20,000 Hz
 *
 * @param p_value - frequency in kilohertz
 * @return consteval frequency - frequency in kilohertz
 */
[[nodiscard]] consteval frequency operator""_kHz(
  unsigned long long p_value) noexcept
{
  const auto value = p_value * std::kilo::num;
  return frequency{ static_cast<std::uint32_t>(value) };
}

/**
 * @brief user defined literals for making frequencies in the megahertz.
 *
 * Example range: 42_MHz == 42,000,000 Hz
 *
 * @param p_value - frequency in megahertz
 * @return consteval frequency - frequency in megahertz
 */
[[nodiscard]] consteval frequency operator""_MHz(
  unsigned long long p_value) noexcept
{
  const auto value = p_value * std::mega::num;
  return frequency{ static_cast<std::uint32_t>(value) };
}
}  // namespace literals
}  // namespace embed
