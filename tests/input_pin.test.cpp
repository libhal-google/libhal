#include <libhal/input_pin.hpp>

#include <boost/ut.hpp>

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

  ~test_input_pin()
  {
  }

private:
  status driver_configure(const settings& p_settings) override
  {
    m_settings = p_settings;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  }
  result<level_t> driver_level() override
  {
    if (m_return_error_status) {
      return hal::new_error();
    }
    return level_t{ .state = true };
  }
};
}  // namespace

void input_pin_test()
{
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
    expect(that % true == result2.value().state);
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
