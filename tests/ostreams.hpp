#pragma once

#include <boost/ut.hpp>
#include <chrono>
#include <iomanip>

#include <libembeddedhal/frequency.hpp>
#include <libembeddedhal/percent.hpp>
#include <libembeddedhal/percentage.hpp>

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
inline std::ostream& operator<<(std::ostream& os, const frequency& p_frequency)
{
  return (os << p_frequency.value_hz << "_Hz");
}

inline std::ostream& operator<<(std::ostream& os, const duty_cycle& p_duty)
{
  return (os << "duty_cycle { high: " << p_duty.high << ","
             << " low: " << p_duty.low << " }");
}

inline std::ostream& operator<<(std::ostream& os, const percent& p_percent)
{
  return (os << "percent { " << std::fixed << std::setprecision(3)
             << static_cast<float>(p_percent) << " : " << std::right
             << std::setfill(' ') << std::setw(10) << p_percent.raw_value()
             << " }");
}
template<std::floating_point float_t>
inline std::ostream& operator<<(std::ostream& os,
                                const percentage<float_t>& p_percent)
{
  return (os << "percentage { " << std::fixed << std::setprecision(15)
             << static_cast<float>(p_percent.value()) << " : " << std::right
             << std::setfill(' ') << std::setw(10) << p_percent.value()
             << " }");
}
}  // namespace hal