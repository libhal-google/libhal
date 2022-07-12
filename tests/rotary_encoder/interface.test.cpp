#include <boost/ut.hpp>
#include <libembeddedhal/rotary_encoder/interface.hpp>

namespace embed {
namespace {
constexpr auto expected_rotations = 1'000'000'000;
class test_rotary_encoder : public embed::rotary_encoder
{
private:
  virtual boost::leaf::result<microrotation> driver_read() noexcept
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
}  // namespace embed
