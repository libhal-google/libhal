#pragma once

#include <algorithm>
#include <chrono>
#include <cinttypes>
#include <limits>
#include <ratio>

#include "math.hpp"
#include "percent.hpp"

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
   * @param p_precent - the target duty cycle percentage
   * @return constexpr duty_cycle - the duty cycle cycle counts
   */
  [[nodiscard]] static constexpr duty_cycle calculate_duty_cycle(
    std::uint32_t p_cycles,
    percent p_precent) noexcept
  {
    // Scale down value based on the integer percentage value in percent
    std::uint32_t high = p_cycles * p_precent;
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
  {}

  /**
   * @brief Get the frequency as an integer
   *
   * @return constexpr auto - frequency value as an integer
   */
  [[nodiscard]] constexpr auto cycles_per_second() const noexcept
  {
    return absolute_value(m_cycles_per_second);
  }

  /**
   * @brief Calculate the frequency divider required to generate the output
   * frequency provided.
   *
   * @param p_target - the target output frequency
   * @return constexpr std::uint32_t - the divider, when applied to this
   * frequency, will achieve the p_target frequency. A value of is an error and
   * 0 indicates that the output frequency is greater than this frequency and
   * there does not exist an integer divider that can produce the output
   * frequency.
   */
  [[nodiscard]] constexpr std::uint32_t divider(
    frequency p_target) const noexcept
  {
    if (p_target.m_cycles_per_second > cycles_per_second()) {
      return 0;
    }
    return rounding_division(cycles_per_second(), p_target.m_cycles_per_second);
  }

  /**
   * @brief Calculate the number of cycles of this frequency within the time
   * duration. This function is meant for timers to determine how many count
   * cycles are needed to reach a paricular time duration at this frequency.
   *
   * @tparam Rep - type of the duration
   * @tparam Period - ratio of the time duration relative to 1 second
   * @param p_duration - the target time duration to get a cycle count from
   * @return constexpr std::uint32_t - the number of cycles of this frequency
   * within the duration.
   */
  template<typename Rep, typename Period>
  [[nodiscard]] constexpr std::uint32_t cycles_per(
    std::chrono::duration<Rep, Period> p_duration) const noexcept
  {
    // Full Equation:
    // =========================================================================
    //
    //                              / ratio_num \_
    //   frequency_hz * |period| * | ----------- |  = cycles
    //                              \ ratio_den /
    //

    // Make sure duration is a positive as the cycle count will always be a
    // positive number.
    const std::intmax_t absolute_duration = std::imaxabs(p_duration.count());
    // The number of cycles between an input frequency and a duration can be
    // found by multiplying the frequency by the time duration. The units cancel
    // out and you get the number of clock intervals within a time period.
    std::intmax_t cycle_count = cycles_per_second() * absolute_duration;
    // The value is multiplied up before division to prevent as much lose of
    // accuracy as possible.
    cycle_count = rounding_division((cycle_count * Period::num), Period::den);

    return static_cast<std::uint32_t>(cycle_count);
  }

  /**
   * @brief
   *
   * @return std::chrono::duration<uint64_t, std::femto> - wavelength of the
   * frequency in femtoseconds.
   */

  /**
   * @brief Calculates and returns the wavelength of the frequency in
   * seconds.
   *
   * @tparam Period - desired period (defaults to std::pico for picoseconds).
   * @return std::chrono::duration<int64_t, Period> - time based wavelength of
   * the frequency.
   */
  template<typename Period = std::pico>
  std::chrono::duration<int64_t, Period> wavelength() const
  {
    // Full Equation (based on the equation in cycles_per()):
    // =========================================================================
    //
    //                /    cycles * ratio_den    \_
    //   |period| =  | ---------------------------|
    //                \ frequency_hz * ratio_num /
    //

    // In this case the number of cycles is 1.
    std::uint64_t numerator = 1 * Period::den;
    std::uint64_t denominator = Period::num * m_cycles_per_second;
    std::uint64_t duration = rounding_division(numerator, denominator);
    return std::chrono::duration<int64_t, Period>(duration);
  }

  /**
   * @brief Calculate the time duration based on the frequency and a number of
   * cycles.
   *
   * @tparam Rep - type of the duration
   * @tparam Period - ratio of the time duration relative to 1 second
   * @param p_cycles - number of cycles within the time duration
   * @return std::chrono::duration<Rep, Period> - time duration based on this
   * frequency and the number of cycles.
   */
  template<typename Rep = std::chrono::nanoseconds::rep,
           typename Period = std::chrono::nanoseconds::period>
  [[nodiscard]] constexpr std::chrono::duration<Rep, Period>
  duration_from_cycles(std::uint64_t p_cycles) const noexcept
  {
    constexpr uint64_t magnitude_delta = std::pico::den / std::nano::den;
    auto picoseconds = wavelength<std::pico>() * p_cycles;
    auto nanoseconds = rounding_division(picoseconds.count(), magnitude_delta);
    return std::chrono::nanoseconds(nanoseconds);
  }

  /**
   * @brief Calculate a duty cycle based on a time duration, the percent
   * and this driving frequency. Typically used for PWM or clock lines with
   * controllable duty cycles for serial communication.
   *
   * @tparam T - containing type of the percent
   * @tparam Rep - containing type of the duration
   * @tparam Period - ratio of the time duration relative to 1 second
   * @param p_duration - target time duration to reach
   * @param p_precent - ratio of the duty cycle high time
   * @return constexpr duty_cycle
   */
  template<typename Rep, typename Period>
  [[nodiscard]] constexpr duty_cycle calculate_duty_cycle(
    std::chrono::duration<Rep, Period> p_duration,
    percent p_precent) const noexcept
  {
    return calculate_duty_cycle(cycles_per(p_duration), p_precent);
  }

  /**
   * @brief Calculate a duty cycle based on a target, the percent
   * and this driving frequency. Typically used for PWM or clock lines with
   * controllable duty cycles for serial communication.
   *
   * @tparam T - containing type of the percent
   * @param p_target - target frequency to reach
   * @param p_precent - ratio of the duty cycle high time
   * @return constexpr duty_cycle
   */
  [[nodiscard]] constexpr duty_cycle calculate_duty_cycle(
    frequency p_target,
    percent p_precent) const noexcept
  {
    return calculate_duty_cycle(divider(p_target), p_precent);
  }

  /**
   * @brief Default operators for <, <=, >, >= and ==
   *
   * @return auto - result of the comparison
   */
  [[nodiscard]] constexpr auto operator<=>(const frequency&) const noexcept =
    default;

  /**
   * @brief Scale up a frequency by an integer factor
   *
   * @tparam Integer - type of the integer
   * @param p_lhs - the frequency to be scaled
   * @param p_rhs - the integer value to scale the frequency by
   * @return constexpr frequency
   */
  template<std::integral Integer>
  [[nodiscard]] constexpr friend frequency operator*(frequency p_lhs,
                                                     Integer p_rhs) noexcept
  {
    return frequency{ p_lhs.cycles_per_second() * p_rhs };
  }

  /**
   * @brief Scale up a frequency by an integer factor
   *
   * @tparam Integer - type of the integer
   * @param p_lhs - the integer value to scale the frequency by
   * @param p_rhs - the frequency to be scaled
   * @return constexpr frequency
   */
  template<std::integral Integer>
  [[nodiscard]] constexpr friend frequency operator*(Integer p_rhs,
                                                     frequency p_lhs) noexcept
  {
    return p_lhs * p_rhs;
  }

  /**
   * @brief Scale down a frequency by an integer factor
   *
   * @tparam Integer - type of the integer
   * @param p_lhs - the frequency to be scaled
   * @param p_rhs - the integer value to scale the frequency by
   * @return constexpr frequency
   */
  template<std::integral Integer>
  [[nodiscard]] constexpr friend frequency operator/(frequency p_lhs,
                                                     Integer p_rhs) noexcept
  {
    return frequency{ rounding_division(p_lhs.cycles_per_second(),
                                        static_cast<std::uint32_t>(p_rhs)) };
  }

  /**
   * @brief Scale down a frequency by an integer factor
   *
   * @param p_input - the input frequency to be divided down to the target
   * frequency with an integer divider
   * @param p_target - the target frequency to reach via an integer divider
   * @return constexpr std::uint32_t - frequency divider value representing
   * the number of cycles in the input that constitute one cycle in the target
   * frequency.
   */
  [[nodiscard]] constexpr friend std::uint32_t operator/(
    frequency p_input,
    frequency p_target) noexcept
  {
    return p_input.divider(p_target);
  }

  /**
   * @brief multiplying a frequency by a time period returns the number of
   * cycles of the input frequency contained within the time duration.
   *
   * @tparam Rep - type of the duration
   * @tparam Period - ratio of the time duration relative to 1 second
   * @param p_input - the input frequency
   * @param p_duration - the target time duration to get a cycle count from
   * @return constexpr std::uint32_t - the number of cycles of this frequency
   * within the duration.
   */
  template<typename Rep, typename Period>
  [[nodiscard]] constexpr friend std::uint32_t operator*(
    frequency p_input,
    std::chrono::duration<Rep, Period> p_duration) noexcept
  {
    return p_input.cycles_per(p_duration);
  }

  /**
   * @brief multiplying a frequency by a time period returns the number of
   * cycles of the input frequency contained within the time duration.
   *
   * @tparam Rep - type of the duration
   * @tparam Period - ratio of the time duration relative to 1 second
   * @param p_input - the input frequency
   * @param p_duration - the target time duration to get a cycle count from
   * @return constexpr std::uint32_t - the number of cycles of this frequency
   * within the duration.
   */
  template<typename Rep, typename Period>
  [[nodiscard]] constexpr friend std::uint32_t operator*(
    std::chrono::duration<Rep, Period> p_duration,
    frequency p_input) noexcept
  {
    return p_input * p_duration;
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
