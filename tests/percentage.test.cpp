#include "ostreams.hpp"
#include <boost/ut.hpp>
#include <libhal/percentage.hpp>

namespace hal {
boost::ut::suite percentage_test = []() {
  using namespace boost::ut;

  "hal::percentage"_test = []() {
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
      constexpr hal::percentage<double> assign_double = 1.5;
      expect(that % 1.0_d == assign_double.value());
      static_assert(1.0f == assign_double.value());
      constexpr hal::percentage<float> assign_float = 1.5f;
      expect(that % 1.0_f == assign_float.value());
      static_assert(1.0f == assign_float.value());
      constexpr hal::percentage<double> assign_double_neg = -1.5;
      expect(that % -1.0_d == assign_double_neg.value());
      static_assert(-1.0 == assign_double_neg.value());
      constexpr hal::percentage<float> assign_float_neg = -1.5f;
      expect(that % -1.0_f == assign_float_neg.value());
      static_assert(-1.0f == assign_float_neg.value());
    };

    "const assignment"_test = []() {
      constexpr hal::percentage<double> assign_double = 1.5;
      hal::percentage<double> assign_double2 = assign_double;
      expect(that % assign_double.value() == assign_double2.value());
    };
    "const copy"_test = []() {
      constexpr hal::percentage<double> assign_double = 1.5;
      hal::percentage<double> assign_double2(assign_double);
      expect(that % assign_double.value() == assign_double2.value());
    };
  };
};
}  // namespace hal