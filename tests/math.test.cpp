#include <boost/ut.hpp>
#include <libhal/math.hpp>

namespace hal {
boost::ut::suite math_test = []() {
  using namespace boost::ut;

  "hal::multiply()"_test = []() {
    "Zero"_test = []() { expect(that % 0 == multiply(0, 0).value()); };
    "One"_test = []() { expect(that % 1 == multiply(1, 1).value()); };
    "Boundaries"_test = []() {
      expect(that % 4294967295UL == multiply(1UL, 4294967295UL).value());
      expect(that % -2147483647L == multiply(-1L, 2147483647L).value());
      expect(that % 2147483648L == multiply(-1L, -2147483648L).value());
    };
    "Exceptions"_test = []() {
      expect(throws([] {
        return multiply(std::uint32_t{ 5U }, std::uint32_t{ 4294967295U })
          .value();
      }));
      expect(throws([] {
        return multiply(std::uint32_t{ 4L }, std::uint32_t{ 1073741824L })
          .value();
      }));
    };
    "Standard Usage"_test = []() {
      expect(that % 75 == multiply(15, 5).value());
      expect(that % -100 == multiply(-10, 10).value());
      expect(that % -4 == multiply(2, -2).value());
      expect(that % -1016379 == multiply(-17, 59787).value());
    };
  };

  "hal::distance()"_test = []() {
    "Zero"_test = []() { expect(that % 0 == distance(0, 0)); };
    "One"_test = []() { expect(that % 1 == distance(0, 1)); };
    "Boundaries"_test = []() {
      expect(that % std::uint32_t{ 4294967294UL } ==
             distance(std::uint32_t{ 1UL },
                      std::numeric_limits<std::uint32_t>::max()));
      expect(that % std::uint32_t{ 4294967295UL } ==
             distance(std::numeric_limits<std::int32_t>::min(),
                      std::numeric_limits<std::int32_t>::max()));
    };
    "Standard Usage"_test = []() {
      expect(that % 10 == distance(15, 5));
      expect(that % 20 == distance(-10, 10));
      expect(that % 4 == distance(2, -2));
      expect(that % 59804 == distance(-17, 59787));
      expect(that % 221200 == distance(222323, 1123));
    };
  };
};
}
