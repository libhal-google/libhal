#include <boost/ut.hpp>
#include <libembeddedhal/rotary_encoder/interface.hpp>

namespace hal {
namespace {
constexpr auto expected_rotations = 1'000'000'000;
class test_rotary_encoder : public hal::rotary_encoder
{
private:
  boost::leaf::result<microrotation> driver_read() noexcept override
  {
    return expected_rotations;
  }
};
}  // namespace

boost::ut::suite rotary_encoder_test = []() {
  using namespace boost::ut;
  // Setup
  test_rotary_encoder test;

  // Exercise
  auto sample = test.read().value();

  // Verify
  expect(expected_rotations == sample);
};
}  // namespace hal
