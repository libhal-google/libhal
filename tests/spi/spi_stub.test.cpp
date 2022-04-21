#include <boost/ut.hpp>
#include <libembeddedhal/spi/spi_stub.hpp>
#include <span>

namespace embed {
boost::ut::suite spi_util_test = []() {
  using namespace boost::ut;

  // Setup
  embed::stub::spi stub;

  expect(bool{ stub.configure({}) });
  expect(bool{ stub.transfer({}, {}, std::byte{ 0xFF }) });
};
}  // namespace embed
