#include <boost/ut.hpp>
#include <libhal/output_pin/mock.hpp>

namespace hal {
boost::ut::suite output_pin_mock_test = []() {
  using namespace boost::ut;

  "hal::mock::output_pin::configure()"_test = []() {
    // Setup
    constexpr hal::output_pin::settings mock_settings_default{};
    constexpr hal::output_pin::settings mock_settings_custom{
      .resistor = pin_resistor::pull_down,
      .open_drain = true,
      .starting_level = false,
    };
    hal::mock::output_pin mock;

    // Exercise
    auto result1 = mock.configure(mock_settings_default);
    auto result2 = mock.configure(mock_settings_custom);

    // Verify
    expect(mock_settings_default ==
           std::get<0>(mock.spy_configure.call_history().at(0)));
    expect(mock_settings_custom ==
           std::get<0>(mock.spy_configure.call_history().at(1)));
    expect(bool{ result1 });
    expect(bool{ result2 });
  };

  "hal::mock::output_pin::driver_level(bool)"_test = []() {
    // Setup
    hal::mock::output_pin mock;

    // Exercise
    auto result1 = mock.level(true);
    auto result2 = mock.level(false);

    // Verify
    expect(bool{ result1 });
    expect(that % true == std::get<0>(mock.spy_level.call_history().at(0)));
    expect(bool{ result2 });
    expect(that % false == std::get<0>(mock.spy_level.call_history().at(1)));
  };

  "hal::mock::output_pin::driver_level()"_test = []() {
    // Setup
    hal::mock::output_pin mock;

    // Exercise
    (void)mock.level(true);
    auto result1 = mock.level();
    (void)mock.level(false);
    auto result2 = mock.level();

    // Verify
    expect(bool{ result1 });
    expect(that % true == result1.value());
    expect(bool{ result2 });
    expect(that % false == result2.value());
  };

  "hal::mock::output_pin::reset()"_test = []() {
    // Setup
    constexpr hal::output_pin::settings mock_settings_default{};
    hal::mock::output_pin mock;
    (void)mock.configure(mock_settings_default);
    (void)mock.level(false);

    // Exercise
    mock.reset();

    // Verify
    expect(that % 0 == mock.spy_configure.call_history().size());
    expect(that % 0 == mock.spy_level.call_history().size());
  };
};
}  // namespace hal
