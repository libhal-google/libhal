#include <boost/ut.hpp>
#include <libhal/timeout.hpp>

namespace hal {
boost::ut::suite timeout_test = []() {
  using namespace boost::ut;

  "hal::delay(timeout)"_test = []() {
    // Setup
    constexpr int timeout_call_limit = 10;
    int counts = 0;
    auto timeout_function = [&counts]() mutable -> status {
      counts++;
      if (counts >= timeout_call_limit) {
        return boost::leaf::new_error(std::errc::timed_out);
      }
      return {};
    };
    using timeout_type = decltype(timeout_function);

    // Exercise
    // Exercise: direct
    auto result = hal::delay<timeout_type>(timeout_function);
    // Exercise: Reset
    counts = 0;
    // Exercise: polymorphic
    result = hal::delay(timeout_function);

    // Verify
    expect(that % timeout_call_limit == counts);
    expect(bool{ result });
  };

  "hal::delay(timeout) returns error"_test = []() {
    // Setup
    auto timeout_function = []() mutable -> status {
      return boost::leaf::new_error();
    };

    // Exercise
    auto result = hal::delay(timeout_function);

    // Verify
    expect(!bool{ result });
  };
};
}  // namespace hal
