#include "ostreams.hpp"
#include <boost/ut.hpp>
#include <libembeddedhal/percentage.hpp>

namespace embed {
boost::ut::suite percentage_test = []() {
  using namespace boost::ut;

  "ctor: zero"_test = []() {
    expect(that % 0.0_d == embed::percentage(0.0).value());
    expect(that % 0.0_f == embed::percentage(0.0f).value());
    embed::percentage<double> assign_double = 0.0;
    expect(that % 0.0_d == assign_double.value());
    embed::percentage<float> assign_float = 0.0f;
    expect(that % 0.0_f == assign_float.value());
  };

  "ctor: standard"_test = []() {
    expect(that % 0.222_d == embed::percentage(0.222).value());
    expect(that % 0.123_d == embed::percentage(0.123).value());
    expect(that % 0.875_d == embed::percentage(0.875).value());
    expect(that % 0.999_d == embed::percentage(0.999).value());
    expect(that % 0.347_d == embed::percentage(0.347).value());
  };

  "ctor: out of bounds"_test = []() {
    expect(that % 1.0_d == embed::percentage(1.5).value());
    expect(that % -1.0_d == embed::percentage(-1.5).value());
    expect(that % 1.0_f == embed::percentage(1.5f).value());
    expect(that % -1.0_f == embed::percentage(-1.5f).value());
    embed::percentage<double> assign_double = 1.5;
    expect(that % 1.0_d == assign_double.value());
    embed::percentage<float> assign_float = 1.5f;
    expect(that % 1.0_f == assign_float.value());
    embed::percentage<double> assign_double_neg = -1.5;
    expect(that % -1.0_d == assign_double_neg.value());
    embed::percentage<float> assign_float_neg = -1.5f;
    expect(that % -1.0_f == assign_float_neg.value());
  };
};
}  // namespace embed
