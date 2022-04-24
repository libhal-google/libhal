#include <boost/ut.hpp>
#include <units/isq/si/international/length.h>
#include <units/isq/si/international/speed.h>  // IWYU pragma: keep
#include <units/isq/si/length.h>
#include <units/isq/si/speed.h>  // IWYU pragma: keep
#include <units/isq/si/time.h>

namespace embed {
using namespace units::isq;
namespace units_testing {
Speed auto avg_speed(Length auto d, Time auto t)
{
  return d / t;
}
}  // namespace units_testing

// This test is here purely to ensure that units compile and work in
// libembeddedhal
boost::ut::suite units_test = []() {
  using namespace boost::ut;

  using namespace units::isq;
  using namespace units::aliases::isq::si::speed;
  using namespace units::aliases::isq::si::length;
  using namespace units::aliases::isq::si::time;
  using units::aliases::isq::si::international::speed::mi_per_h;

  Speed auto v1 =
    units_testing::avg_speed(km<std::int64_t>(1000), h<std::int64_t>(3));
  Speed auto v2 = units_testing::avg_speed(
    si::length<si::international::mile>(140), si::time<si::hour>(2));

  expect(v1 == km_per_h<std::int64_t>(333));
  expect(v2 == mi_per_h(70));
};
}  // namespace embed
