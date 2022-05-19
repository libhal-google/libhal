#include <boost/ut.hpp>
#include <libembeddedhal/servo/interface.hpp>

namespace embed {
namespace {
constexpr mrad expected_value(100);
mrad actual_value;

class test_servo : public embed::servo
{
private:
  boost::leaf::result<void> driver_angle(embed::angle p_angle)
  {
    actual_value = p_angle;
    return {};
  }
};
}  // namespace

boost::ut::suite servo_test = []() {
  using namespace boost::ut;
  // Setup
  test_servo test;

  // Exercise
  test.angle(expected_value).value();

  // Verify
  expect(expected_value.number() == actual_value.number());
};
}  // namespace embed
