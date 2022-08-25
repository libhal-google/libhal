#pragma once

#include <algorithm>
#include <concepts>

#include "math.hpp"

namespace hal {
/**
 * @brief A type representing a percentage value
 *
 * This class guarantees that its value is always [-1.0, 1.0], thus bounds
 * checking is never required when retrieving its internal floating point value
 * either via `value()`, `static_cast<float>` or `static_cast<double>`
 *
 * @tparam float_t - floating point representation
 */
template<std::floating_point float_t>
class percentage_t
{
public:
  constexpr static float_t max = +1.0;
  constexpr static float_t min = -1.0;
  constexpr static float_t zero = 0.0;

  constexpr percentage_t()
    : m_value(0.0)
  {
  }

  template<std::floating_point U>
  constexpr percentage_t(U p_value)
    : m_value(0.0)
  {
    set_and_constrain(static_cast<float_t>(p_value));
  }
  constexpr percentage_t& operator=(const percentage_t& p_percent)
  {
    m_value = p_percent.value();
    return *this;
  }
  constexpr percentage_t& operator=(const percentage_t&& p_percent)
  {
    m_value = p_percent.value();
    return *this;
  }
  constexpr percentage_t& operator=(float_t p_value)
  {
    set_and_constrain(p_value);
    return *this;
  }
  constexpr percentage_t(const percentage_t& p_percent)
  {
    m_value = p_percent.value();
  }
  constexpr percentage_t(const percentage_t&& p_percent)
  {
    m_value = p_percent.value();
  }
  constexpr auto value() const
  {
    return m_value;
  }
  constexpr explicit operator double() const
  {
    return static_cast<double>(m_value);
  }
  constexpr explicit operator float() const
  {
    return static_cast<float>(m_value);
  }
  constexpr bool equals(percentage_t p_other, float_t p_epsilon = 1e-9f) const
  {
    return hal::equals(m_value, p_other.value(), static_cast<float>(p_epsilon));
  }
  constexpr bool operator<=>(const percentage_t& p_percent) const = default;
  constexpr bool operator==(const percentage_t& p_percent) const
  {
    return equals(p_percent);
  }

private:
  constexpr void set_and_constrain(float_t p_value)
  {
    m_value = std::clamp(p_value, min, max);
  }
  float_t m_value{ 0.0 };
};

using percentage = percentage_t<float>;
}  // namespace hal
