#include <boost/ut.hpp>
#include <libembeddedhal/pwm/mock.hpp>

namespace hal {
boost::ut::suite pwm_mock_test = []() {
  using namespace boost::ut;

  "hal::mock::pwm::configure()"_test = []() {
    // Setup
    constexpr hal::pwm::settings expected1 = {
      .frequency = frequency(1'000),
    };
    constexpr hal::pwm::settings expected2 = {
      .frequency = frequency(10'000),
    };
    hal::mock::pwm mock;
    mock.spy_configure.trigger_error_on_call(3);

    // Exercise + Verify
    expect(bool{ mock.configure(expected1) });
    expect(expected1 == std::get<0>(mock.spy_configure.call_history().at(0)));

    expect(bool{ mock.configure(expected2) });
    expect(expected2 == std::get<0>(mock.spy_configure.call_history().at(1)));

    expect(!mock.configure(expected2));
    expect(expected2 == std::get<0>(mock.spy_configure.call_history().at(2)));
  };

  "hal::mock::pwm::duty_cycle()"_test = []() {
    // Setup
    constexpr auto expected1 = percent::from_ratio(1, 2);
    constexpr auto expected2 = percent::from_ratio(1, 4);
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
