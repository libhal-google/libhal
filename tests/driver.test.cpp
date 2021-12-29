#include <boost/ut.hpp>
#include <libembeddedhal/driver.hpp>
#include <cstdio>

namespace embed {
boost::ut::suite driver_test = []() {
  using namespace boost::ut;

  class driver_impl : public driver<> {
    public:

    boost::leaf::result<void> driver_initialize() {
      return {};
    }
  };

  driver_impl impl;
  impl.initialize();
};
}
