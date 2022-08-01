#include <boost/ut.hpp>
#include <libhal/motor/mock.hpp>

namespace hal {
boost::ut::suite motor_mock_test = []() {
  using namespace boost::ut;

  // Setup
  hal::mock::motor mock;
  constexpr auto expected1 = percentage<config::float_type>(0.5);
  constexpr auto expected2 = percentage<config::float_type>(0.25);
  mock.spy_power.trigger_error_on_call(3);

  // Exercise + Verify
  expect(bool{ mock.power(expected1) });
  expect(expected1.value() ==
         std::get<0>(mock.spy_power.call_history().at(0)).value());

  expect(bool{ mock.power(expected2) });
  expect(expected2.value() ==
         std::get<0>(mock.spy_power.call_history().at(1)).value());

  expect(!mock.power(expected2));
  expect(expected2.value() ==
         std::get<0>(mock.spy_power.call_history().at(2)).value());
};
}  // namespace hal
