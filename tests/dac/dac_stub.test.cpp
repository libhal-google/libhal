#include <boost/ut.hpp>
#include <libembeddedhal/dac/dac_stub.hpp>

namespace embed {
boost::ut::suite dac_stub_test = []() {
  using namespace boost::ut;

  // Setup
  embed::stub::dac stub;

  // Exercise + Verify
  expect(bool{ stub.write({}) });
};
}  // namespace embed
