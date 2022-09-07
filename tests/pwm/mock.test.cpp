#include <boost/ut.hpp>
#include <libhal/pwm/mock.hpp>

namespace hal {
boost::ut::suite pwm_mock_test = []() {
  using namespace boost::ut;

  "hal::mock::pwm::frequency()"_test = []() {
    // Setup
    constexpr auto expected1 = hertz(1'000);
    constexpr auto expected2 = hertz(10'000);
    hal::mock::pwm mock;
    mock.spy_frequency.trigger_error_on_call(3);

    // Exercise + Verify
    expect(bool{ mock.frequency(expected1) });
    expect(expected1 == std::get<0>(mock.spy_frequency.call_history().at(0)));

    expect(bool{ mock.frequency(expected2) });
    expect(expected2 == std::get<0>(mock.spy_frequency.call_history().at(1)));

    expect(!mock.frequency(expected2));
    expect(expected2 == std::get<0>(mock.spy_frequency.call_history().at(2)));
  };

  "hal::mock::pwm::duty_cycle()"_test = []() {
    // Setup
    constexpr auto expected1 = float(0.5);
    constexpr auto expected2 = float(0.25);
    hal::mock::pwm mock;
    mock.spy_duty_cycle.trigger_error_on_call(3);

    // Exercise + Verify
    expect(bool{ mock.duty_cycle(expected1) });
    expect(expected1 == std::get<0>(mock.spy_duty_cycle.call_history().at(0)));

    expect(bool{ mock.duty_cycle(expected2) });
    expect(expected2 == std::get<0>(mock.spy_duty_cycle.call_history().at(1)));

    expect(!mock.duty_cycle(expected2));
    expect(expected2 == std::get<0>(mock.spy_duty_cycle.call_history().at(2)));
  };
};
}  // namespace hal
