#include <boost/ut.hpp>
#include <libembeddedhal/timeout.hpp>

namespace embed {
boost::ut::suite timeout_test = []() {
  using namespace boost::ut;

  "embed::delay(timeout)"_test = []() {
    // Setup
    constexpr int timeout_call_limit = 10;
    int counts = 0;
    auto timeout_function = [&counts]() mutable -> boost::leaf::result<void> {
      counts++;
      if (counts >= timeout_call_limit) {
        return boost::leaf::new_error(embed::error::timeout{});
      }
      return {};
    };
    using timeout_type = decltype(timeout_function);

    // Exercise
    // Exercise: direct
    auto result = embed::delay<timeout_type>(timeout_function);
    // Exercise: Reset
    counts = 0;
    // Exercise: polymorphic
    result = embed::delay(timeout_function);

    // Verify
    expect(that % timeout_call_limit == counts);
    expect(bool{ result });
  };

  "embed::delay(timeout) returns error"_test = []() {
    // Setup
    auto timeout_function = []() mutable -> boost::leaf::result<void> {
      return boost::leaf::new_error();
    };

    // Exercise
    auto result = embed::delay(timeout_function);

    // Verify
    expect(!bool{ result });
  };
};
}  // namespace embed
