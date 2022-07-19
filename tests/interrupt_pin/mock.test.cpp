#include <deque>

#include <boost/ut.hpp>
#include <libembeddedhal/interrupt_pin/mock.hpp>

namespace embed {
boost::ut::suite interrupt_pin_mock_test = []() {
  using namespace boost::ut;

  "embed::mock::interrupt_pin::configure()"_test = []() {
    // Setup
    constexpr embed::interrupt_pin_interface::settings mock_settings_default{};
    constexpr embed::interrupt_pin_interface::settings mock_settings_custom{
      .resistor = pin_resistor::pull_down,
    };
    embed::mock::interrupt_pin mock;

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
  "embed::mock::interrupt_pin::set() + level()"_test = []() {
    // Setup
    embed::mock::interrupt_pin mock;
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
  "embed::mock::interrupt_pin::detach_interrupt()"_test = []() {
    // Setup
    embed::mock::interrupt_pin mock;

    // Exercise
    (void)mock.detach_interrupt();

    // Verify
    expect(that % true ==
           std::get<0>(mock.spy_detach_interrupt.call_history().at(0)));
  };
  "embed::mock::interrupt_pin::attach_interrupt()"_test = []() {
    // Setup
    const std::function<void(void)> expected_callback = []() {};
    const embed::interrupt_pin_interface::trigger_edge expected_falling =
      embed::interrupt_pin_interface::trigger_edge::falling;
    const embed::interrupt_pin_interface::trigger_edge expected_rising =
      embed::interrupt_pin_interface::trigger_edge::rising;
    const embed::interrupt_pin_interface::trigger_edge expected_both =
      embed::interrupt_pin_interface::trigger_edge::both;
    embed::mock::interrupt_pin mock;

    // Exercise
    auto result1 = mock.attach_interrupt(expected_callback, expected_falling);
    auto result2 = mock.attach_interrupt(expected_callback, expected_rising);
    auto result3 = mock.attach_interrupt(expected_callback, expected_both);

    // Verify
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(bool{ result3 });
    expect(expected_falling ==
           std::get<1>(mock.spy_attach_interrupt.call_history().at(0)));
    expect(expected_rising ==
           std::get<1>(mock.spy_attach_interrupt.call_history().at(1)));
    expect(expected_both ==
           std::get<1>(mock.spy_attach_interrupt.call_history().at(2)));
  };
  "embed::mock::interrupt_pin::reset()"_test = []() {
    // Setup
    constexpr embed::interrupt_pin_interface::settings mock_settings_default{};
    const std::function<void(void)> expected_callback = []() {};
    const embed::interrupt_pin_interface::trigger_edge expected_trigger =
      embed::interrupt_pin_interface::trigger_edge::falling;
    embed::mock::interrupt_pin mock;
    (void)mock.configure(mock_settings_default);
    expect(that % 1 == mock.spy_configure.call_history().size());
    (void)mock.attach_interrupt(expected_callback, expected_trigger);
    expect(that % 1 == mock.spy_attach_interrupt.call_history().size());
    (void)mock.detach_interrupt();
    expect(that % 1 == mock.spy_detach_interrupt.call_history().size());

    // Exercise
    mock.reset();

    // Verify
    expect(that % 0 == mock.spy_configure.call_history().size());
    expect(that % 0 == mock.spy_attach_interrupt.call_history().size());
    expect(that % 0 == mock.spy_detach_interrupt.call_history().size());
  };
};
}  // namespace embed
