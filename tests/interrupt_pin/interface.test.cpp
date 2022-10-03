#include <boost/ut.hpp>
#include <libhal/interrupt_pin/interface.hpp>

namespace hal {
namespace {
constexpr hal::interrupt_pin::settings expected_settings{
  .resistor = pin_resistor::pull_down,
  .trigger = interrupt_pin::trigger_edge::rising,
};

class test_interrupt_pin : public hal::interrupt_pin
{
public:
  settings m_settings{};
  std::function<handler> m_callback{};
  bool m_return_error_status{ false };

private:
  status driver_configure(const settings& p_settings) noexcept override
  {
    m_settings = p_settings;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };
  void driver_on_trigger(std::function<handler> p_callback) noexcept override
  {
    m_callback = p_callback;
  };
};
}  // namespace

boost::ut::suite interrupt_pin_test = []() {
  using namespace boost::ut;
  "interrupt_pin interface test"_test = []() {
    // Setup
    test_interrupt_pin test;
    int counter = 0;
    const std::function<void(bool)> expected_callback = [&counter](bool) {
      counter++;
    };

    // Exercise
    auto result = test.configure(expected_settings);
    test.on_trigger(expected_callback);
    test.m_callback(false);

    // Verify
    expect(bool{ result });
    expect(expected_settings == test.m_settings);
    expect(that % 1 == counter);
  };

  "interrupt_pin errors test"_test = []() {
    // Setup
    test_interrupt_pin test;
    test.m_return_error_status = true;

    // Exercise
    auto result = test.configure(expected_settings);

    // Verify
    expect(!bool{ result });
  };
};
}  // namespace hal
