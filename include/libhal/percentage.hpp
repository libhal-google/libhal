#pragma once

#include <algorithm>
#include <concepts>

#include "config.hpp"

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
template<std::floating_point float_t = float>
class percentage
{
public:
  constexpr static float_t max = +1.0;
  constexpr static float_t min = -1.0;
  constexpr static float_t zero = 0.0;

  constexpr percentage(float_t p_value)
    : m_value(0.0)
  {
    set_and_constrain(p_value);
  }
  constexpr percentage(const percentage<float_t>& p_percent)
  {
    m_value = p_percent.value();
  }
  constexpr percentage(const percentage<float_t>&& p_percent)
  {
    m_value = p_percent.value();
  }
  constexpr percentage<float_t>& operator=(const percentage<float_t>& p_percent)
  {
    m_value = p_percent.value();
    return *this;
  }
  constexpr percentage<float_t>&& operator=(
    const percentage<float_t>&& p_percent)
  {
    m_value = p_percent.value();
    return *this;
  }
  constexpr percentage& operator=(float_t p_value)
  {
    set_and_constrain(p_value);
    return *this;
  }
  auto value() const
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

private:
  constexpr void set_and_constrain(float_t p_value)
  {
    m_value = std::clamp(p_value, min, max);
  }
  float_t m_value{ 0.0 };
};
}  // namespace hal
