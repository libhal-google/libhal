#include <boost/ut.hpp>
#include <libhal/dac/mock.hpp>

namespace hal {
boost::ut::suite dac_mock_test = []() {
  using namespace boost::ut;

  // Setup
  hal::mock::dac mock;
  constexpr auto expected1 = float(0.5);
  constexpr auto expected2 = float(0.25);
  mock.spy_write.trigger_error_on_call(3);

  // Exercise + Verify
  expect(bool{ mock.write(expected1) });
  expect(expected1 == std::get<0>(mock.spy_write.call_history().at(0)));

  expect(bool{ mock.write(expected2) });
  expect(expected2 == std::get<0>(mock.spy_write.call_history().at(1)));

  expect(!mock.write(expected2));
  expect(expected2 == std::get<0>(mock.spy_write.call_history().at(2)));
};
}  // namespace hal
