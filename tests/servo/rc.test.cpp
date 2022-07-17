#include <boost/ut.hpp>
#include <libembeddedhal/pwm/mock.hpp>
#include <libembeddedhal/servo/rc.hpp>

#include "ostreams.hpp"

namespace embed {
boost::ut::suite rc_servo_test = []() {
  using namespace boost::ut;

  "embed::servo::rc_servo::create"_test = []() {
    // Setup
    embed::mock::pwm pwm0;
    embed::mock::pwm pwm1;
    embed::mock::pwm pwm2;

    // Exercise
    // use defaults
    auto servo0 = embed::rc_servo::create(pwm0);
    // 100Hz (or 10ms per update) with 500us being max negative start and 2500us
    // being max positive.
    auto servo1 = embed::rc_servo::create<100, 500, 2500>(pwm1);
    pwm2.spy_configure.trigger_error_on_call(1);
    auto servo2 = embed::rc_servo::create(pwm2);

    // Verify
    expect(bool{ servo0 });
    expect(bool{ servo1 });
    expect(!servo2);
  };

  "embed::servo::rc_servo::position"_test = []() {
    // Setup
    constexpr auto expected0 = percent::from_ratio(5, 100);
    constexpr auto expected1 = percent::from_ratio(1, 10);
    constexpr auto expected2 = percent::from_ratio(15, 100);
    constexpr auto expected3 = percent::from_ratio(2, 10);
    constexpr auto expected4 = percent::from_ratio(1, 4);

    constexpr auto percent_neg_100 = embed::percent::from_ratio(-1, 1);
    constexpr auto percent_neg_50 = embed::percent::from_ratio(-1, 2);
    constexpr auto percent_0 = embed::percent::from_ratio(0, 1);
    constexpr auto percent_50 = embed::percent::from_ratio(1, 2);
    constexpr auto percent_100 = embed::percent::from_ratio(1, 1);

    embed::mock::pwm pwm;
    auto servo = embed::rc_servo::create<100, 500, 2500>(pwm).value();

    // Exercise
    auto result0 = servo.position(percent_neg_100);
    auto result1 = servo.position(percent_neg_50);
    auto result2 = servo.position(percent_0);
    auto result3 = servo.position(percent_50);
    auto result4 = servo.position(percent_100);

    // Verify
    expect(bool{ result0 });
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(bool{ result3 });
    expect(bool{ result4 });

    expect(that % expected0 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(0)));
    expect(that % expected1 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(1)));
    expect(that % expected2 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(2)));
    expect(that % expected3 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(3)));
    expect(that % expected4 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(4)));
  };
};
}  // namespace embed
