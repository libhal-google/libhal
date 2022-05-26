#include <boost/ut.hpp>
#include <libembeddedhal/adc/mock.hpp>

namespace embed {
boost::ut::suite adc_mock_test = []() {
  using namespace boost::ut;

  // Setup
  auto expected = embed::percent::from_ratio(1, 2);
  embed::mock::adc mock(expected);

  // Exercise + Verify
  expect(that % expected.raw_value() == mock.read().value().raw_value());
};
}  // namespace embed
