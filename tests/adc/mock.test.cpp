#include <boost/ut.hpp>
#include <libembeddedhal/adc/mock.hpp>

namespace hal {
boost::ut::suite adc_mock_test = []() {
  using namespace boost::ut;

  // Setup
  auto expected1 = hal::percent::from_ratio(1, 4);
  auto expected2 = hal::percent::from_ratio(1, 2);
  auto expected3 = hal::percent::from_ratio(1, 1);
  hal::mock::adc mock(expected1);

  // Exercise + Verify
  expect(that % expected1.raw_value() == mock.read().value().raw_value());
  mock.set(expected2);
  expect(that % expected2.raw_value() == mock.read().value().raw_value());
  mock.set(expected3);
  expect(that % expected3.raw_value() == mock.read().value().raw_value());
};
}  // namespace hal
