#include <boost/ut.hpp>
#include <libhal/servo/interface.hpp>

#include <libhal/testing.hpp>

namespace hal {
namespace {
constexpr auto expected_value = float(0.5);

class test_servo : public hal::servo
{
public:
  float m_passed_position{};
  bool m_return_error_status{ false };

private:
  status driver_position(float p_position) noexcept override
  {
    m_passed_position = p_position;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  }
};
}  // namespace

boost::ut::suite servo_test = []() {
  using namespace boost::ut;
  "servo interface test"_test = []() {
    // Setup
    test_servo test;

    // Exercise
    auto result = test.position(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % expected_value == test.m_passed_position);
  };

  "servo errors test"_test = []() {
    // Setup
    test_servo test;
    test.m_return_error_status = true;

    // Exercise
    auto result = test.position(expected_value);

    // Verify
    expect(!bool{ result });
  };
};
}  // namespace hal
