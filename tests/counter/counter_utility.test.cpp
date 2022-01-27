#include <boost/ut.hpp>
#include <libembeddedhal/counter/counter_utility.hpp>

namespace embed {
boost::ut::suite counter_utility_test = []() {
  using namespace boost::ut;
  using namespace std::literals;
  embed::this_thread::sleep_for(0ns);
  embed::this_thread::uptime();
};
}  // namespace embed
