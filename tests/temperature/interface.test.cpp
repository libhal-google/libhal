#include <boost/ut.hpp>
#include <libembeddedhal/temperature/interface.hpp>

namespace embed {
namespace {
constexpr auto expected_temperature = nK(1'000'000'000);
class test_temperature_sensor : public embed::temperature_sensor
{
private:
  virtual boost::leaf::result<temperature> driver_read() noexcept
  {
    return expected_temperature;
  }
};
}  // namespace

boost::ut::suite temperature_test = []() {
  using namespace boost::ut;
  // Setup
  test_temperature_sensor test;

  // Exercise
  auto sample = test.read().value();

  // Verify
  expect(expected_temperature == sample);
};
}  // namespace embed
