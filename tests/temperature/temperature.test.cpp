#include <boost/ut.hpp>
#include <libembeddedhal/temperature/temperature.hpp>
#include <libembeddedhal/temperature/temperature_util.hpp>

namespace embed {
namespace {
constexpr auto expected_temperature = nK(1'000'000'000);
class test_temperature_sensor : public embed::temperature_sensor
{
private:
  virtual boost::leaf::result<std::span<temperature>> driver_read(
    std::span<temperature> p_sample) noexcept
  {
    std::ranges::fill(p_sample, expected_temperature);
    return p_sample;
  }
};
}  // namespace

boost::ut::suite temperature_test = []() {
  using namespace boost::ut;

  "interface { embed::temperature_sensor }"_test = []() {
    // Setup
    test_temperature_sensor test;
    std::array<temperature, 2> sample{};

    // Exercise
    auto response_span = test.read(sample).value();

    // Verify
    expect(that % 2 == response_span.size());
    expect(that % sample.data() == response_span.data());
    expect(expected_temperature == sample[0]);
  };

  "read<N>(embed::temperature_sensor)"_test = []() {
    // Setup
    test_temperature_sensor test;

    // Exercise
    auto response_array = read<3>(test).value();

    // Verify
    expect(that % 3 == response_array.size());
    expect(expected_temperature == response_array[0]);
    expect(expected_temperature == response_array[1]);
    expect(expected_temperature == response_array[2]);
  };
};
}  // namespace embed
