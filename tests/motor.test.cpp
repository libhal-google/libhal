#include <boost/ut.hpp>
#include <libhal/motor.hpp>

namespace hal {
namespace {
constexpr auto expected_value = float(0.5);

class test_motor : public hal::motor
{
public:
  float m_power{};
  bool m_return_error_status{ false };

  ~test_motor()
  {
  }

private:
  status driver_power(float p_power) noexcept override
  {
    m_power = p_power;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };
};
}  // namespace

void motor_test()
{
  using namespace boost::ut;
  "motor interface test"_test = []() {
    // Setup
    test_motor test;

    // Exercise
    auto result = test.power(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % expected_value == test.m_power);
  };

  "motor errors test"_test = []() {
    // Setup
    test_motor test;
    test.m_return_error_status = true;

    // Exercise
    auto result = test.power(expected_value);

    // Verify
    expect(!bool{ result });
  };
};
}  // namespace hal