#include "ostreams.hpp"
#include <boost/ut.hpp>
#include <libhal/percentage.hpp>

namespace hal {
boost::ut::suite percentage_test = []() {
  using namespace boost::ut;

  "ctor: zero"_test = []() {
    expect(that % 0.0_d == hal::percentage(0.0).value());
    expect(that % 0.0_f == hal::percentage(0.0f).value());
    hal::percentage<double> assign_double = 0.0;
    expect(that % 0.0_d == assign_double.value());
    hal::percentage<float> assign_float = 0.0f;
    expect(that % 0.0_f == assign_float.value());
  };

  "ctor: standard"_test = []() {
    expect(that % 0.222_d == hal::percentage(0.222).value());
    expect(that % 0.123_d == hal::percentage(0.123).value());
    expect(that % 0.875_d == hal::percentage(0.875).value());
    expect(that % 0.999_d == hal::percentage(0.999).value());
    expect(that % 0.347_d == hal::percentage(0.347).value());
  };

  "ctor: out of bounds"_test = []() {
    expect(that % 1.0_d == hal::percentage(1.5).value());
    expect(that % -1.0_d == hal::percentage(-1.5).value());
    expect(that % 1.0_f == hal::percentage(1.5f).value());
    expect(that % -1.0_f == hal::percentage(-1.5f).value());
    hal::percentage<double> assign_double = 1.5;
    expect(that % 1.0_d == assign_double.value());
    hal::percentage<float> assign_float = 1.5f;
    expect(that % 1.0_f == assign_float.value());
    hal::percentage<double> assign_double_neg = -1.5;
    expect(that % -1.0_d == assign_double_neg.value());
    hal::percentage<float> assign_float_neg = -1.5f;
    expect(that % -1.0_f == assign_float_neg.value());
  };
};
}  // namespace hal
