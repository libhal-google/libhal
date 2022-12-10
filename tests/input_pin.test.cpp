#include <boost/ut.hpp>
#include <libhal/input_pin.hpp>

namespace hal {
namespace {
constexpr hal::input_pin::settings expected_settings{
  .resistor = pin_resistor::pull_down,
};
class test_input_pin : public hal::input_pin
{
public:
  settings m_settings{};
  bool m_return_error_status{ false };

private:
  status driver_configure(const settings& p_settings) noexcept override
  {
    m_settings = p_settings;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  }
  result<bool> driver_level() noexcept override
  {
    if (m_return_error_status) {
      return hal::new_error();
    }
    return true;
  }
};
}  // namespace

boost::ut::suite input_pin_test = []() {
  using namespace boost::ut;
  "input_pin interface test"_test = []() {
    // Setup
    test_input_pin test;

    // Exercise
    auto result1 = test.configure(expected_settings);
    auto result2 = test.level();

    // Verify
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(expected_settings.resistor == test.m_settings.resistor);
    expect(that % true == result2.value());
  };

  "input_pin errors test"_test = []() {
    // Setup
    test_input_pin test;
    test.m_return_error_status = true;

    // Exercise
    auto result1 = test.configure(expected_settings);
    auto result2 = test.level();

    // Verify
    expect(!bool{ result1 });
    expect(!bool{ result2 });
  };
};
}  // namespace hal
