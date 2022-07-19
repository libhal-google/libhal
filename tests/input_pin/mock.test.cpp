#include <deque>

#include <boost/ut.hpp>
#include <libembeddedhal/input_pin/mock.hpp>

namespace embed {
boost::ut::suite input_pin_mock_test = []() {
  using namespace boost::ut;

  "embed::mock::inputput_pin::configure()"_test = []() {
    // Setup
    constexpr embed::input_pin_interface::settings mock_settings_default{};
    constexpr embed::input_pin_interface::settings mock_settings_custom{
      .resistor = pin_resistor::pull_down,
    };
    embed::mock::input_pin mock;

    // Exercise
    auto result1 = mock.configure(mock_settings_default);
    auto result2 = mock.configure(mock_settings_custom);

    // Verify
    expect(bool{ result1 });
    expect(mock_settings_default ==
           std::get<0>(mock.spy_configure.call_history().at(0)));
    expect(bool{ result2 });
    expect(mock_settings_custom ==
           std::get<0>(mock.spy_configure.call_history().at(1)));
  };
  "embed::mock::input_pin::set() + level()"_test = []() {
    // Setup
    embed::mock::input_pin mock;
    std::deque inputs{ true, false, true };
    std::queue queue(inputs);

    // Exercise
    mock.set(queue);

    // Verify
    expect(that % true == mock.level().value());
    expect(that % false == mock.level().value());
    expect(that % true == mock.level().value());
    expect(throws([&mock] { mock.level().value(); }));
  };
  "embed::mock::input_pin::reset()"_test = []() {
    // Setup
    constexpr embed::input_pin_interface::settings mock_settings_default{};
    embed::mock::input_pin mock;
    (void)mock.configure(mock_settings_default);
    expect(that % 1 == mock.spy_configure.call_history().size());

    // Exercise
    mock.reset();

    // Verify
    expect(that % 0 == mock.spy_configure.call_history().size());
  };
};
}  // namespace embed
