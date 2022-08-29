#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <span>

#include <libhal/percentage.hpp>
#include <libhal/units.hpp>

template<typename Rep, typename Period>
inline std::ostream& operator<<(
  std::ostream& os,
  const std::chrono::duration<Rep, Period>& p_duration)
{
  return (os << p_duration.count() << " * (" << Period::num << "/"
             << Period::den << ")s");
}

template<typename T, size_t size>
inline std::ostream& operator<<(std::ostream& os,
                                const std::array<T, size>& p_array)
{
  os << "{";
  for (const auto& element : p_array) {
    os << element << ", ";
  }
  return os << "}\n";
}
template<typename T>
inline std::ostream& operator<<(std::ostream& os, const std::span<T>& p_array)
{
  os << "{";
  for (const auto& element : p_array) {
    os << element << ", ";
  }
  return os << "}\n";
}

template<typename T, size_t size>
inline bool operator==(const std::array<T, size>& p_array,
                       const std::span<T>& p_span) noexcept
{
  if (p_span.size() != size) {
    return false;
  }

  return std::equal(p_array.begin(), p_array.end(), p_span.begin());
}

template<typename T, size_t size>
inline bool operator!=(const std::array<T, size>& p_array,
                       const std::span<T>& p_span) noexcept
{
  return !(p_array == p_span);
}

template<typename T, size_t size>
bool operator==(const std::array<T, size>& p_lhs,
                const std::array<T, size>& p_rhs)
{
  return std::equal(p_lhs.begin(), p_lhs.end(), p_rhs.begin());
}

template<typename T, size_t size>
bool operator!=(const std::array<T, size>& p_lhs,
                const std::array<T, size>& p_rhs)
{
  return !(p_lhs == p_rhs);
}

namespace hal {
template<std::floating_point float_t>
inline std::ostream& operator<<(std::ostream& os,
                                const percentage_t<float_t>& p_percent)
{
  return (os << "percentage { " << std::fixed << std::setprecision(15)
             << static_cast<float>(p_percent.value()) << " : " << std::right
             << std::setfill(' ') << std::setw(10) << p_percent.value()
             << " }");
}
}  // namespace hal
