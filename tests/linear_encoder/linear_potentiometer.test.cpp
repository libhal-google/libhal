#include <boost/ut.hpp>
#include <libembeddedhal/adc/adc_stub.hpp>
#include <libembeddedhal/linear_encoder/linear_potentiometer.hpp>

namespace embed {
boost::ut::suite linear_pot_test = []() {
  using namespace boost::ut;

  "embed::linear_potentiometer::read() basic test"_test = []() {
    // Setup
    embed::stub::adc stub(embed::percent::from_ratio(1, 2));
    embed::linear_potentiometer::settings settings;
    settings.p_min_distance = units::aliases::isq::si::nm<std::int64_t>(20);
    settings.p_max_distance = units::aliases::isq::si::nm<std::int64_t>(120);
    settings.p_min_adc_output = embed::percent::from_ratio(0, 1);
    settings.p_max_adc_output = embed::percent::from_ratio(1, 1);
    embed::linear_potentiometer test(stub, settings);

    // Exercise + Verify
    expect(70 == test.read().value().number());
  };

  "embed::linear_potentiometer::read() default settings"_test = []() {
    // Setup
    embed::stub::adc stub(embed::percent::from_ratio(1, 2));
    embed::linear_potentiometer::settings settings;
    embed::linear_potentiometer test(stub, settings);

    // Exercise + Verify
    expect(0 == test.read().value().number());
  };

  "embed::linear_potentiometer::read() max percent"_test = []() {
    // Setup
    embed::stub::adc stub(embed::percent::from_ratio(1, 1));
    embed::linear_potentiometer::settings settings;
    settings.p_min_distance = units::aliases::isq::si::nm<std::int64_t>(0);
    settings.p_max_distance = units::aliases::isq::si::nm<std::int64_t>(100);
    settings.p_min_adc_output = embed::percent::from_ratio(0, 1);
    settings.p_max_adc_output = embed::percent::from_ratio(1, 1);
    embed::linear_potentiometer test(stub, settings);

    // Exercise + Verify
    expect(100 == test.read().value().number());
  };

  "embed::linear_potentiometer::read() min percent"_test = []() {
    // Setup
    embed::stub::adc stub(embed::percent::from_ratio(0, 1));
    embed::linear_potentiometer::settings settings;
    settings.p_min_distance = units::aliases::isq::si::nm<std::int64_t>(0);
    settings.p_max_distance = units::aliases::isq::si::nm<std::int64_t>(100);
    settings.p_min_adc_output = embed::percent::from_ratio(0, 1);
    settings.p_max_adc_output = embed::percent::from_ratio(1, 1);
    embed::linear_potentiometer test(stub, settings);

    // Exercise + Verify
    expect(0 == test.read().value().number());
  };

  "embed::linear_potentiometer::read() negative percents"_test = []() {
    // Setup
    embed::stub::adc stub(embed::percent::from_ratio(-1, 1));
    embed::linear_potentiometer::settings settings;
    settings.p_min_distance = units::aliases::isq::si::nm<std::int64_t>(0);
    settings.p_max_distance = units::aliases::isq::si::nm<std::int64_t>(100);
    settings.p_min_adc_output = embed::percent::from_ratio(-1, 1);
    settings.p_max_adc_output = embed::percent::from_ratio(0, 1);
    embed::linear_potentiometer test(stub, settings);

    // Exercise + Verify
    expect(0 == test.read().value().number());
  };

  "embed::linear_potentiometer::read() negative distances"_test = []() {
    // Setup
    embed::stub::adc stub(embed::percent::from_ratio(1, 2));
    embed::linear_potentiometer::settings settings;
    settings.p_min_distance = units::aliases::isq::si::nm<std::int64_t>(-200);
    settings.p_max_distance = units::aliases::isq::si::nm<std::int64_t>(-100);
    settings.p_min_adc_output = embed::percent::from_ratio(0, 1);
    settings.p_max_adc_output = embed::percent::from_ratio(1, 1);
    embed::linear_potentiometer test(stub, settings);

    // Exercise + Verify
    expect(-150 == test.read().value().number());
  };

  "embed::linear_potentiometer::read() adc percent bigger than max percent"_test =
    []() {
      // Setup
      embed::stub::adc stub(embed::percent::from_ratio(1, 1));
      embed::linear_potentiometer::settings settings;
      settings.p_min_distance = units::aliases::isq::si::nm<std::int64_t>(0);
      settings.p_max_distance = units::aliases::isq::si::nm<std::int64_t>(100);
      settings.p_min_adc_output = embed::percent::from_ratio(0, 1);
      settings.p_max_adc_output = embed::percent::from_ratio(1, 2);
      embed::linear_potentiometer test(stub, settings);

      // Exercise + Verify
      expect(200 == test.read().value().number());
    };

  "embed::linear_potentiometer::read() adc percent smaller than min percent"_test =
    []() {
      // Setup
      embed::stub::adc stub(embed::percent::from_ratio(1, 4));
      embed::linear_potentiometer::settings settings;
      settings.p_min_distance = units::aliases::isq::si::nm<std::int64_t>(0);
      settings.p_max_distance = units::aliases::isq::si::nm<std::int64_t>(100);
      settings.p_min_adc_output = embed::percent::from_ratio(1, 2);
      settings.p_max_adc_output = embed::percent::from_ratio(1, 1);
      embed::linear_potentiometer test(stub, settings);

      // Exercise + Verify
      expect(-50 == test.read().value().number());
    };
};
}  // namespace embed
