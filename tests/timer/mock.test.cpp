#include <boost/ut.hpp>
#include <libembeddedhal/timer/mock.hpp>

namespace embed {
boost::ut::suite timer_mock_test = []() {
  using namespace boost::ut;

  // Setup
  embed::mock::timer mock;
  const std::function<void(void)> expected_callback = []() {};
  const std::chrono::nanoseconds expected_delay = {};

  // Exercise + Verify
  expect(false == mock.is_running().value());
  expect(true == std::get<0>(mock.spy_is_running.call_history().at(0)));
  expect(bool{ mock.schedule(expected_callback, expected_delay) });
  expect(bool{ std::get<0>(mock.spy_schedule.call_history().at(0)) });
  expect(expected_delay == std::get<1>(mock.spy_schedule.call_history().at(0)));
  expect(true == mock.is_running().value());
  expect(true == std::get<0>(mock.spy_is_running.call_history().at(1)));
  expect(bool{ mock.clear() });
  expect(true == std::get<0>(mock.spy_clear.call_history().at(0)));
};
}  // namespace embed
