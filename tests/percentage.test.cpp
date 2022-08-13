#include "ostreams.hpp"
#include <boost/ut.hpp>
#include <libhal/percentage.hpp>

namespace hal {
boost::ut::suite percentage_test = []() {
  using namespace boost::ut;

  "hal::percentage_t"_test = []() {
    "ctor: zero"_test = []() {
      expect(that % 0.0_d == hal::percentage_t<double>(0.0).value());
      expect(that % 0.0_f == hal::percentage_t<float>(0.0f).value());
      hal::percentage_t<double> assign_double = 0.0;
      expect(that % 0.0_d == assign_double.value());
      hal::percentage_t<float> assign_float = 0.0f;
      expect(that % 0.0_f == assign_float.value());
    };

    "ctor: standard"_test = []() {
      expect(that % 0.222_d == hal::percentage_t<double>(0.222).value());
      expect(that % 0.123_d == hal::percentage_t<double>(0.123).value());
      expect(that % 0.875_d == hal::percentage_t<double>(0.875).value());
      expect(that % 0.999_d == hal::percentage_t<double>(0.999).value());
      expect(that % 0.347_d == hal::percentage_t<double>(0.347).value());
    };

    "ctor: out of bounds"_test = []() {
      expect(that % 1.0_d == hal::percentage_t<double>(1.5).value());
      expect(that % -1.0_d == hal::percentage_t<double>(-1.5).value());
      expect(that % 1.0_f == hal::percentage_t<float>(1.5f).value());
      expect(that % -1.0_f == hal::percentage_t<float>(-1.5f).value());
      constexpr hal::percentage_t<double> assign_double = 1.5;
      expect(that % 1.0_d == assign_double.value());
      static_assert(1.0f == assign_double.value());
      constexpr hal::percentage_t<float> assign_float = 1.5f;
      expect(that % 1.0_f == assign_float.value());
      static_assert(1.0f == assign_float.value());
      constexpr hal::percentage_t<double> assign_double_neg = -1.5;
      expect(that % -1.0_d == assign_double_neg.value());
      static_assert(-1.0 == assign_double_neg.value());
      constexpr hal::percentage_t<float> assign_float_neg = -1.5f;
      expect(that % -1.0_f == assign_float_neg.value());
      static_assert(-1.0f == assign_float_neg.value());
    };

    "const assignment"_test = []() {
      constexpr hal::percentage_t<double> assign_double = 1.5;
      hal::percentage_t<double> assign_double2 = assign_double;
      expect(that % assign_double.value() == assign_double2.value());
    };
    "const copy"_test = []() {
      constexpr hal::percentage_t<double> assign_double = 1.5;
      hal::percentage_t<double> assign_double2(assign_double);
      expect(that % assign_double.value() == assign_double2.value());
    };
    "const equals zero"_test = []() {
      // Setup
      constexpr auto value1 = hal::percentage_t<float>(0.0f);
      constexpr auto value2 = hal::percentage_t<float>(0.1f - 0.1f);
      constexpr auto epsilon = 0.000001f;

      // Exercise + verify
      expect(value1.equals(value2, epsilon));
    };
    "const equals boundaries"_test = []() {
      // Setup
      constexpr auto value1 = hal::percentage_t<float>(0.2f + 0.8f);
      constexpr auto value2 = hal::percentage_t<float>(0.5F + 0.5f);
      constexpr auto epsilon = 0.000001f;

      // Exercise + verify
      expect(value1.equals(value2, epsilon));
    };
    "const equals standard"_test = []() {
      // Setup
      constexpr auto value1 = hal::percentage_t<float>(0.15f + 0.15f);
      constexpr auto value2 = hal::percentage_t<float>(0.2f + 0.1f);
      constexpr auto epsilon = 0.000001f;

      // Exercise + verify
      expect(value1.equals(value2, epsilon));
    };
    "const equals standard default epsilon"_test = []() {
      // Setup
      constexpr auto value1 = hal::percentage_t<float>(0.15f + 0.15f);
      constexpr auto value2 = hal::percentage_t<float>(0.2f + 0.1f);

      // Exercise + verify
      expect(value1.equals(value2));
    };
    "const equals standard default epsilon not equal"_test = []() {
      // Setup
      constexpr auto value1 = hal::percentage_t<float>(0.3f);
      constexpr auto value2 = hal::percentage_t<float>(0.4f);

      // Exercise + verify
      expect(false == value1.equals(value2));
    };
    "const equals standard double"_test = []() {
      // Setup
      constexpr auto value1 = hal::percentage_t<double>(0.15 + 0.15);
      constexpr auto value2 = hal::percentage_t<double>(0.2 + 0.1);
      constexpr auto epsilon = 0.000001;

      // Exercise + verify
      expect(value1.equals(value2, epsilon));
    };
    "const equals standard not equal"_test = []() {
      // Setup
      constexpr auto value1 = hal::percentage_t<float>(0.3f);
      constexpr auto value2 = hal::percentage_t<float>(0.4f);
      constexpr auto epsilon = 0.000001f;

      // Exercise + verify
      expect(false == value1.equals(value2, epsilon));
    };
    "operator== zero"_test = []() {
      // Setup
      constexpr auto value1 = hal::percentage_t<float>(0.0f);
      constexpr auto value2 = hal::percentage_t<float>(0.1f - 0.1f);

      // Exercise + verify
      expect(value1 == value2);
    };
    "operator== boundaries"_test = []() {
      // Setup
      constexpr auto value1 = hal::percentage_t<float>(0.2f + 0.8f);
      constexpr auto value2 = hal::percentage_t<float>(0.5F + 0.5f);

      // Exercise + verify
      expect(value1 == value2);
    };
    "operator== standard"_test = []() {
      // Setup
      constexpr auto value1 = hal::percentage_t<float>(0.15f + 0.15f);
      constexpr auto value2 = hal::percentage_t<float>(0.2f + 0.1f);

      // Exercise + verify
      expect(value1 == value2);
    };
    "operator== standard"_test = []() {
      // Setup
      constexpr auto value1 = hal::percentage_t<double>(0.15 + 0.15);
      constexpr auto value2 = hal::percentage_t<double>(0.2 + 0.1);

      // Exercise + verify
      expect(value1 == value2);
    };
    "operator== standard not equal"_test = []() {
      // Setup
      constexpr auto value1 = hal::percentage_t<float>(0.3f);
      constexpr auto value2 = hal::percentage_t<float>(0.4f);

      // Exercise + verify
      expect(value1 != value2);
    };
    "operator== standard not equal"_test = []() {
      // Setup
      constexpr auto value1 = hal::percentage_t<float>(0.3001f);
      constexpr auto value2 = hal::percentage_t<float>(0.3002f);

      // Exercise + verify
      expect(value1 != value2);
    };
  };
};
}  // namespace hal
