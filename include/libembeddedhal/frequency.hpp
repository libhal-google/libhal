#pragma once

#include <algorithm>
#include <chrono>
#include <cinttypes>
#include <limits>
#include <ratio>
#include <span>
#include <stdio.h>

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
   * @brief Divider selection mode for achieving a target frequency.
   */
  enum class selection_mode
  {
    /// Restrict dividers to acheive frequencies above target frequency.
    higher,
    /// Restrict dividers to acheive frequencies below target frequency.
    lower,
    /// Do not restrict dividers.
    closest
  };

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
   * nanoseconds duration cannot exceed 4294967297000000000ns or ~136.1 average
   * Gregorian years otherwise the output of this function is not defined.
   *
   * @param p_duration - the amount of time to convert to cycles
   * @return boost::leaf::result<std::int64_t> - number of cycles
   * @throws std::errc::result_out_of_range - if the calculated cycle count
   * exceeds std::int64_t.
   */
  [[nodiscard]] boost::leaf::result<std::int64_t> cycles_per(
    std::chrono::nanoseconds p_duration) const noexcept
  {
    // Full Equation:
    // =========================================================================
    //
    //                              / ratio_num \_
    //   frequency_hz * |period| * | ----------- |  = cycles
    //                              \ ratio_den /
    //
    // std::chrono::nanoseconds::period::num == 1
    // std::chrono::nanoseconds::period::den == 1,000,000,000

    using uint128_t = math::wide_integer::uint128_t;

    constexpr uint128_t numerator = decltype(p_duration)::period::num;
    constexpr uint128_t denominator = decltype(p_duration)::period::den;
    // Storing 64-bit value in a uint128_t for later computation, no truncation
    // possible.
    const uint128_t duration = p_duration.count();
    // Duration contains at most a 64-bit number, cycles_per_second() is a
    // 32-bit number, and numerator is always the value 1.
    //
    // To contain the maximum resultant possible requires storage within an
    // integer of size 96-bits, and thus will fit within a uint128_t variable,
    // no overflow checks required.
    const uint128_t count = duration * cycles_per_second() * numerator;
    const uint128_t cycle_count = rounding_division(count, denominator);

    // The nanoseconds denominator is 1,000,000,000. It's log2 value of ~29.897
    // meaning one would need a 30 bit integer to represent it. Dividing a
    // number by it will reduce the number of its it needs to be stored in by
    // ~30. Because 96-bits - 30-bits is equal to 66-bits, it is possible that
    // the resultant will not fit within a 64-bit unsigned integer, and thus a
    // bounds check is required.
    if (cycle_count > std::numeric_limits<std::int64_t>::max()) {
      return boost::leaf::new_error(std::errc::result_out_of_range);
    }

    return static_cast<std::uint64_t>(cycle_count);
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
   * @brief Calculate the time duration based on the frequency and a number of
   * cycles.
   *
   * @param p_cycles - number of cycles within the time duration
   * @return boost::leaf::result<std::chrono::nanoseconds> - time duration based
   * on this frequency and the number of cycles or
   * `std::errc::result_out_of_range`.
   */
  [[nodiscard]] boost::leaf::result<std::chrono::nanoseconds>
  duration_from_cycles(std::uint64_t p_cycles) const noexcept
  {
    // Full Equation (based on the equation in cycles_per()):
    // =========================================================================
    //
    //                /    cycles * ratio_den    \_
    //   |period| =  | ---------------------------|
    //                \ frequency_hz * ratio_num /
    //
    using uint128_t = math::wide_integer::uint128_t;

    uint128_t numerator = BOOST_LEAF_CHECK(multiply_with_overflow_detection(
      uint128_t{ p_cycles }, uint128_t{ std::nano::den }));

    uint128_t denominator = cycles_per_second() * std::nano::num;
    uint128_t nanoseconds = rounding_division(numerator, denominator);

    if (nanoseconds > std::numeric_limits<std::int64_t>::max()) {
      return boost::leaf::new_error(std::errc::result_out_of_range);
    }

    return std::chrono::nanoseconds(static_cast<std::int64_t>(nanoseconds));
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
    std::chrono::nanoseconds p_duration,
    percent p_percent) const noexcept
  {
    std::uint64_t cycles = BOOST_LEAF_CHECK(cycles_per(p_duration));
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
   * @brief Calculate closest resulting frequency to target when using one of
   * the provided dividers.
   *
   * @param p_target - The target frequency to acheive by using one of the
   * dividers
   * @param p_dividers - The set of available dividers
   * @param p_selection_mode - The selection mode for dividers. This can
   * restrict the dividers to result in a frequency less than or equal to the
   * target, higher than or equal to the target or not restrict the dividers at
   * all and get the closest value to the target.
   * @return std::optional<frequency> - The resulting frequency or std::nullopt
   * if no solution is found.
   */
  [[nodiscard]] constexpr std::optional<frequency> closest(
    frequency p_target,
    std::span<std::uint32_t> p_dividers,
    selection_mode p_selection_mode)
  {
    auto is_applicable = [&](std::uint32_t p_candidate) -> bool {
      auto resulting_frequency = (*this / p_candidate);

      switch (p_selection_mode) {
        case selection_mode::lower:
          return resulting_frequency <= p_target;
        case selection_mode::higher:
          return resulting_frequency >= p_target;
        case selection_mode::closest:
          return true;
        default:
          return false;
      }
    };

    auto cost = [&](std::uint32_t p_candidate) -> std::uint32_t {
      return distance((*this / p_candidate).cycles_per_second(),
                      p_target.cycles_per_second());
    };

    auto best =
      std::find_if(p_dividers.begin(), p_dividers.end(), is_applicable);
    for (auto candidate = best; candidate != p_dividers.end(); candidate++) {
      if (is_applicable(*candidate) && cost(*candidate) < cost(*best)) {
        best = candidate;
      }
    }

    if (best == p_dividers.end()) {
      return std::nullopt;
    }
    return *this / *best;
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
