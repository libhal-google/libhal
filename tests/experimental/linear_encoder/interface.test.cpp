#include <boost/ut.hpp>
#include <libembeddedhal/linear_encoder/interface.hpp>

namespace hal {
namespace {
constexpr auto expected_linear_position = length(1000000);
class test_linear_encoder : public hal::linear_encoder
{
private:
  boost::leaf::result<length> driver_read() noexcept override
  {
    return expected_linear_position;
  }
};
}  // namespace

boost::ut::suite linear_encoder_test = []() {
  using namespace boost::ut;
  // Setup
  test_linear_encoder test;

  // Exercise
  auto sample = test.read().value();

  // Verify
  expect(expected_linear_position == sample);
};
}  // namespace hal
