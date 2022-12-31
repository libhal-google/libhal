#include <libhal/pwm.hpp>

#include <boost/ut.hpp>

namespace hal {
namespace {
constexpr auto expected_frequency = hertz(1'000);
constexpr auto expected_duty_cycle = float(0.5);

class test_pwm : public hal::pwm
{
public:
  hertz m_frequency{};
  float m_duty_cycle{};
  bool m_return_error_status{ false };

  ~test_pwm()
  {
  }

private:
  status driver_frequency(hertz p_frequency) override
  {
    m_frequency = p_frequency;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };
  status driver_duty_cycle(float p_duty_cycle) override
  {
    m_duty_cycle = p_duty_cycle;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };
};
}  // namespace

void pwm_test()
{
  using namespace boost::ut;
  "pwm interface test"_test = []() {
    // Setup
    test_pwm test;

    // Exercise
    auto result1 = test.frequency(expected_frequency);
    auto result2 = test.duty_cycle(expected_duty_cycle);

    // Verify
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(that % expected_frequency == test.m_frequency);
    expect(that % expected_duty_cycle == test.m_duty_cycle);
  };

  "pwm errors test"_test = []() {
    // Setup
    test_pwm test;
    test.m_return_error_status = true;

    // Exercise
    auto result1 = test.frequency(expected_frequency);
    auto result2 = test.duty_cycle(expected_duty_cycle);

    // Verify
    expect(!bool{ result1 });
    expect(!bool{ result2 });
  };
};
}  // namespace hal
