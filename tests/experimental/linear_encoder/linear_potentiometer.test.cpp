#include <boost/ut.hpp>
#include <libembeddedhal/adc/mock.hpp>
#include <libembeddedhal/linear_potentiometer/interface.hpp>

namespace hal {
boost::ut::suite linear_pot_test = []() {
  using namespace boost::ut;

  "hal::linear_potentiometer::read() default settings"_test = []() {
    // Setup
    hal::mock::adc mock(hal::percent::from_ratio(1, 2));
    hal::linear_potentiometer::settings settings;
    hal::linear_potentiometer test(mock, settings);

    // Exercise + Verify
    expect(that % 0 == test.read().value());
  };

  "hal::linear_potentiometer::read() positive percent range"_test = []() {
    // Setup
    hal::mock::adc mock(hal::percent::from_ratio(0, 1));
    hal::linear_potentiometer::settings settings;
    settings.p_min_distance = 1;
    settings.p_max_distance = 100;
    settings.p_min_adc_output = hal::percent::from_ratio(0, 1);
    settings.p_max_adc_output = hal::percent::from_ratio(1, 1);
    hal::linear_potentiometer test(mock, settings);

    // Exercise + Verify
    expect(that % 1 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 4));
    expect(that % 26 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 2));
    expect(that % 51 == test.read().value());
    mock.set(hal::percent::from_ratio(3, 4));
    expect(that % 76 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 1));
    expect(that % 100 == test.read().value());
    mock.set(hal::percent::from_ratio(2, 1));
    expect(that % 100 == test.read().value());
    mock.set(hal::percent::from_ratio(-1, 2));
    expect(that % 1 == test.read().value());
  };

  "hal::linear_potentiometer::read() negative percent range"_test = []() {
    // Setup
    hal::mock::adc mock(hal::percent::from_ratio(-1, 1));
    hal::linear_potentiometer::settings settings;
    settings.p_min_distance = 0;
    settings.p_max_distance = 100;
    settings.p_min_adc_output = hal::percent::from_ratio(-1, 1);
    settings.p_max_adc_output = hal::percent::from_ratio(0, 1);
    hal::linear_potentiometer test(mock, settings);

    // Exercise + Verify
    expect(that % 0 == test.read().value());
    mock.set(hal::percent::from_ratio(-1, 4));
    expect(that % 75 == test.read().value());
    mock.set(hal::percent::from_ratio(-1, 2));
    expect(that % 50 == test.read().value());
    mock.set(hal::percent::from_ratio(-3, 4));
    expect(that % 25 == test.read().value());
    mock.set(hal::percent::from_ratio(0, 1));
    expect(that % 100 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 2));
    expect(that % 100 == test.read().value());
    mock.set(hal::percent::from_ratio(-2, 1));
    expect(that % 0 == test.read().value());
  };

  "hal::linear_potentiometer::read() negative distances"_test = []() {
    // Setup
    hal::mock::adc mock(hal::percent::from_ratio(0, 1));
    hal::linear_potentiometer::settings settings;
    settings.p_min_distance = -200;
    settings.p_max_distance = -100;
    settings.p_min_adc_output = hal::percent::from_ratio(0, 1);
    settings.p_max_adc_output = hal::percent::from_ratio(1, 1);
    hal::linear_potentiometer test(mock, settings);

    // Exercise + Verify
    expect(that % -200 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 4));
    expect(that % -175 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 2));
    expect(that % -150 == test.read().value());
    mock.set(hal::percent::from_ratio(3, 4));
    expect(that % -125 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 1));
    expect(that % -100 == test.read().value());
    mock.set(hal::percent::from_ratio(-1, 2));
    expect(that % -200 == test.read().value());
    mock.set(hal::percent::from_ratio(2, 1));
    expect(that % -100 == test.read().value());
  };

  "hal::linear_potentiometer::read() large positive range"_test = []() {
    // Setup
    hal::mock::adc mock(hal::percent::from_ratio(0, 1));
    hal::linear_potentiometer::settings settings;
    settings.p_min_distance = 0;
    settings.p_max_distance = 400000000;
    settings.p_min_adc_output = hal::percent::from_ratio(0, 1);
    settings.p_max_adc_output = hal::percent::from_ratio(1, 1);
    hal::linear_potentiometer test(mock, settings);

    // Exercise + Verify
    expect(that % 0 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 4));
    // division arithmetic
    expect(that % 100000000 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 2));
    expect(that % 200000000 == test.read().value());
    mock.set(hal::percent::from_ratio(3, 4));
    expect(that % 300000000 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 1));
    expect(that % 400000000 == test.read().value());
    mock.set(hal::percent::from_ratio(2, 1));
    expect(that % 400000000 == test.read().value());
    mock.set(hal::percent::from_ratio(-1, 2));
    expect(that % 0 == test.read().value());
  };

  "hal::linear_potentiometer::read() large negative range"_test = []() {
    // Setup
    hal::mock::adc mock(hal::percent::from_ratio(0, 1));
    hal::linear_potentiometer::settings settings;
    settings.p_min_distance = -400000000;
    settings.p_max_distance = 0;
    settings.p_min_adc_output = hal::percent::from_ratio(0, 1);
    settings.p_max_adc_output = hal::percent::from_ratio(1, 1);
    hal::linear_potentiometer test(mock, settings);

    // Exercise + Verify
    expect(that % -400000000 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 4));
    // TODO: Fix with issue #273, expected value adjusted to work with current
    // division arithmetic
    expect(that % -300000000 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 2));
    expect(that % -200000000 == test.read().value());
    mock.set(hal::percent::from_ratio(3, 4));
    expect(that % -100000000 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 1));
    expect(that % 0 == test.read().value());
    mock.set(hal::percent::from_ratio(2, 1));
    expect(that % 0 == test.read().value());
    mock.set(hal::percent::from_ratio(-1, 2));
    expect(that % -400000000 == test.read().value());
  };

  "hal::linear_potentiometer::read() range exceeds max integer value"_test =
    []() {
      // Setup
      hal::mock::adc mock(hal::percent::from_ratio(0, 1));
      hal::linear_potentiometer::settings settings;
      settings.p_min_distance = -400000000;
      settings.p_max_distance = 400000000;
      settings.p_min_adc_output = hal::percent::from_ratio(0, 1);
      settings.p_max_adc_output = hal::percent::from_ratio(1, 1);
      hal::linear_potentiometer test(mock, settings);

      // Exercise + Verify
      expect(that % -400000000 == test.read().value());
      mock.set(hal::percent::from_ratio(1, 4));
      expect(that % -200000000 == test.read().value());
      mock.set(hal::percent::from_ratio(3, 4));
      expect(that % 200000000 == test.read().value());
      mock.set(hal::percent::from_ratio(1, 1));
      expect(that % 400000000 == test.read().value());
      mock.set(hal::percent::from_ratio(2, 1));
      expect(that % 400000000 == test.read().value());
      mock.set(hal::percent::from_ratio(-1, 2));
      expect(that % -400000000 == test.read().value());
    };

  "hal::linear_potentiometer::read() range in meters"_test = []() {
    // Setup
    hal::mock::adc mock(hal::percent::from_ratio(0, 1));
    hal::linear_potentiometer::settings settings;
    settings.p_min_distance = 100'000'000;
    settings.p_max_distance = 200'000'000;
    settings.p_min_adc_output = hal::percent::from_ratio(0, 1);
    settings.p_max_adc_output = hal::percent::from_ratio(1, 1);
    hal::linear_potentiometer test(mock, settings);

    // Exercise + Verify
    expect(that % 100'000'000 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 4));
    expect(that % 125'000'000 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 2));
    expect(that % 150'000'000 == test.read().value());
    mock.set(hal::percent::from_ratio(3, 4));
    expect(that % 175'000'000 == test.read().value());
    mock.set(hal::percent::from_ratio(1, 1));
    expect(that % 200'000'000 == test.read().value());
    mock.set(hal::percent::from_ratio(2, 1));
    expect(that % 200'000'000 == test.read().value());
    mock.set(hal::percent::from_ratio(-1, 2));
    expect(that % 100'000'000 == test.read().value());
  };
};
}  // namespace hal
