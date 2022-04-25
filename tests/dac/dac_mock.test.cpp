#include <boost/ut.hpp>
#include <libembeddedhal/dac/dac_mock.hpp>

namespace embed {
boost::ut::suite dac_mock_test = []() {
  using namespace boost::ut;

  // Setup
  embed::mock::dac mock;
  constexpr auto expected1 = percent::from_ratio(1, 2);
  constexpr auto expected2 = percent::from_ratio(1, 4);
  mock.spy_write.trigger_error_on_call(3);

  // Exercise + Verify
  expect(bool{ mock.write(expected1) });
  expect(expected1 == std::get<0>(mock.spy_write.call_history().at(0)));

  expect(bool{ mock.write(expected2) });
  expect(expected2 == std::get<0>(mock.spy_write.call_history().at(1)));

  expect(!mock.write(expected2));
  expect(expected2 == std::get<0>(mock.spy_write.call_history().at(2)));
};
}  // namespace embed
