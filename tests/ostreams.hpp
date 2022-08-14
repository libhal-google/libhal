#pragma once

#include <boost/ut.hpp>
#include <chrono>
#include <iomanip>

#include <libhal/percentage.hpp>

namespace {
template<typename Rep, typename Period>
inline std::ostream& operator<<(
  std::ostream& os,
  const std::chrono::duration<Rep, Period>& p_duration)
{
  return (os << p_duration.count() << " * (" << Period::num << "/"
             << Period::den << ")s");
}
}  // namespace

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