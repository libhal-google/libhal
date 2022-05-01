#include <algorithm>
#include <boost/ut.hpp>
#include <libembeddedhal/accelerometer/accelerometer.hpp>
#include <libembeddedhal/accelerometer/accelerometer_util.hpp>

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
  virtual boost::leaf::result<std::span<sample>> driver_read(
    std::span<sample> p_samples) noexcept
  {
    std::ranges::fill(p_samples, expected_sample);
    return p_samples;
  }
};
}  // namespace

boost::ut::suite accelerometer_test = []() {
  using namespace boost::ut;

  "interface { embed::accelerometer }"_test = []() {
    // Setup
    test_accelerometer test;
    std::array<embed::accelerometer::sample, 1> sample;

    // Exercise
    auto response_span = test.read(sample).value();

    // Verify
    expect(that % 1 == response_span.size());
    expect(that % sample.data() == response_span.data());
    expect(expected_sample == sample[0]);
  };

  "read<N>(embed::accelerometer)"_test = []() {
    // Setup
    test_accelerometer test;

    // Exercise
    auto response_array = read<3>(test).value();

    // Verify
    expect(that % 3 == response_array.size());
    expect(expected_sample == response_array[0]);
    expect(expected_sample == response_array[1]);
    expect(expected_sample == response_array[2]);
  };
};
}  // namespace embed
