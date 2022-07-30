#include <boost/ut.hpp>
#include <libembeddedhal/accelerometer/interface.hpp>

namespace hal {
namespace {
constexpr auto expected_sample = hal::accelerometer::sample{
  .full_scale = 1'000'000'000,
  .axis = {
    .x = percent::from_ratio(1, 2),
    .y = percent::from_ratio(1, 3),
    .z = percent::from_ratio(1, 4),
  },
};

class test_accelerometer : public hal::accelerometer
{
private:
  boost::leaf::result<sample> driver_read() noexcept override
  {
    return expected_sample;
  }
};
}  // namespace

boost::ut::suite accelerometer_test = []() {
  using namespace boost::ut;
  // Setup
  test_accelerometer test;

  // Exercise
  auto sample = test.read().value();

  // Verify
  expect(expected_sample == sample);
};
}  // namespace hal
