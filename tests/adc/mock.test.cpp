#include <boost/ut.hpp>
#include <libhal/adc/mock.hpp>

namespace hal {
boost::ut::suite adc_mock_test = []() {
  using namespace boost::ut;

  // Setup
  auto expected1 = percentage<config::float_type>(0.25);
  auto expected2 = percentage<config::float_type>(0.5);
  auto expected3 = percentage<config::float_type>(1.0);
  hal::mock::adc mock(expected1);

  // Exercise + Verify
  expect(that % expected1.value() == mock.read().value().value());
  mock.set(expected2);
  expect(that % expected2.value() == mock.read().value().value());
  mock.set(expected3);
  expect(that % expected3.value() == mock.read().value().value());
};
}  // namespace hal
