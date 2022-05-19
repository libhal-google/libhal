#include <boost/ut.hpp>
#include <libembeddedhal/adc/stub.hpp>

namespace embed {
boost::ut::suite adc_stub_test = []() {
  using namespace boost::ut;

  // Setup
  auto expected = embed::percent::from_ratio(1, 2);
  embed::stub::adc stub(expected);

  // Exercise + Verify
  expect(that % expected.raw_value() == stub.read().value().raw_value());
};
}  // namespace embed
