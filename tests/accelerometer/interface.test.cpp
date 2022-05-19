#include <boost/ut.hpp>
#include <libembeddedhal/accelerometer/interface.hpp>

namespace embed {
namespace {
constexpr auto expected_sample = embed::accelerometer::sample{
  .full_scale = nm_per_s2(1'000'000'000),
  .axis = {
    .x = percent::from_ratio(1, 2),
    .y = percent::from_ratio(1, 3),
    .z = percent::from_ratio(1, 4),
  },
};

class test_accelerometer : public embed::accelerometer
{
private:
  virtual boost::leaf::result<sample> driver_read() noexcept
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
}  // namespace embed
