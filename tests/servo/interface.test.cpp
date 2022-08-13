#include <boost/ut.hpp>
#include <libhal/servo/interface.hpp>

#include "../ostreams.hpp"

namespace hal {
namespace {
constexpr auto expected_value = percentage(0.5);

class test_servo : public hal::servo
{
public:
  float_t m_passed_position;

private:
  status driver_position(percentage p_position) noexcept override
  {
    m_passed_position = p_position.value();
    return {};
  }
};
}  // namespace

boost::ut::suite servo_test = []() {
  using namespace boost::ut;
  // Setup
  test_servo test;

  // Exercise
  auto result = test.position(expected_value);

  // Verify
  expect(bool{ result });
  expect(that % expected_value.value() == test.m_passed_position);
};
}  // namespace hal
