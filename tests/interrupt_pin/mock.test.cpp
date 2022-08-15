#include <deque>

#include <boost/ut.hpp>
#include <libhal/interrupt_pin/mock.hpp>

namespace hal {
boost::ut::suite interrupt_pin_mock_test = []() {
  using namespace boost::ut;

  "hal::mock::interrupt_pin::configure()"_test = []() {
    // Setup
    constexpr hal::interrupt_pin::settings mock_settings_default{};
    constexpr hal::interrupt_pin::settings mock_settings_custom{
      .resistor = pin_resistor::pull_down,
      .trigger = interrupt_pin::trigger_edge::rising,
    };
    hal::mock::interrupt_pin mock;

    // Exercise
    auto result1 = mock.configure(mock_settings_default);
    auto result2 = mock.configure(mock_settings_custom);

    // Verify
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(mock_settings_default.resistor ==
           std::get<0>(mock.spy_configure.call_history().at(0)).resistor);
    expect(mock_settings_custom.resistor ==
           std::get<0>(mock.spy_configure.call_history().at(1)).resistor);
  };
  "hal::mock::interrupt_pin::on_trigger()"_test = []() {
    // Setup
    int counter = 0;
    const std::function<void(bool)> expected_callback = [&counter](bool) {
      counter++;
    };
    hal::mock::interrupt_pin mock;

    // Exercise
    mock.on_trigger(expected_callback);
    mock.on_trigger(expected_callback);
    mock.on_trigger(expected_callback);
    auto callback1 = std::get<0>(mock.spy_on_trigger.call_history().at(0));
    auto callback2 = std::get<0>(mock.spy_on_trigger.call_history().at(1));
    auto callback3 = std::get<0>(mock.spy_on_trigger.call_history().at(2));

    // Verify
    callback1(false);
    expect(that % 1 == counter);

    callback2(false);
    expect(that % 2 == counter);

    callback3(false);
    expect(that % 3 == counter);
  };
  "hal::mock::interrupt_pin::reset()"_test = []() {
    // Setup
    constexpr hal::interrupt_pin::settings mock_settings_default{};
    const std::function<void(bool)> expected_callback = [](bool) {};
    hal::mock::interrupt_pin mock;
    (void)mock.configure(mock_settings_default);
    expect(that % 1 == mock.spy_configure.call_history().size());
    (void)mock.on_trigger(expected_callback);
    expect(that % 1 == mock.spy_on_trigger.call_history().size());

    // Exercise
    mock.reset();

    // Verify
    expect(that % 0 == mock.spy_configure.call_history().size());
    expect(that % 0 == mock.spy_on_trigger.call_history().size());
  };
};
}  // namespace hal
