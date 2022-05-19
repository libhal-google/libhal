#include <boost/ut.hpp>
#include <libembeddedhal/timer/stub.hpp>

namespace embed {
boost::ut::suite timer_stub_test = []() {
  using namespace boost::ut;

  // Setup
  embed::stub::timer stub;

  // Exercise + Verify
  expect(that % false == stub.is_running().value());
  expect(bool{ stub.schedule({}, {}) });
  expect(that % true == stub.is_running().value());
  expect(bool{ stub.clear() });
  expect(that % false == stub.is_running().value());
};
}  // namespace embed
