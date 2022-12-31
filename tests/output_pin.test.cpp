#include <libhal/output_pin.hpp>

#include <boost/ut.hpp>

namespace hal {
namespace {
constexpr hal::output_pin::settings expected_settings{
  .resistor = pin_resistor::pull_down,
  .open_drain = true,
};
class test_output_pin : public hal::output_pin
{
public:
  settings m_settings{};
  bool m_driver_level{};
  bool m_return_error_status{ false };

  ~test_output_pin()
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
  };
  status driver_level(bool p_high) override
  {
    m_driver_level = p_high;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };
  result<bool> driver_level() override
  {
    if (m_return_error_status) {
      return hal::new_error();
    }
    return m_driver_level;
  };
};
}  // namespace

void output_pin_test()
{
  using namespace boost::ut;
  "output_pin interface test"_test = []() {
    // Setup
    test_output_pin test;

    // Exercise
    auto result1 = test.configure(expected_settings);
    auto result2 = test.level(true);
    auto result3 = test.level();

    // Verify
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(bool{ result3 });
    expect(expected_settings.open_drain == test.m_settings.open_drain);
    expect(expected_settings.resistor == test.m_settings.resistor);
    expect(that % true == test.m_driver_level);
    expect(that % true == result3.value());
  };

  "output_pin errors test"_test = []() {
    // Setup
    test_output_pin test;
    test.m_return_error_status = true;

    // Exercise
    auto result1 = test.configure(expected_settings);
    auto result2 = test.level(true);
    auto result3 = test.level();

    // Verify
    expect(!bool{ result1 });
    expect(!bool{ result2 });
    expect(!bool{ result3 });
  };
};
}  // namespace hal
