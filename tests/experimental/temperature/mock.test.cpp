#include <deque>

#include <boost/ut.hpp>
#include <libembeddedhal/temperature/mock.hpp>

namespace embed {
boost::ut::suite temperature_mock_test = []() {
  using namespace boost::ut;

  "embed::mock::temperature::read()"_test = []() {
    // Setup
    embed::mock::temperature_sensor mock;
    auto expected1 = 0;
    auto expected2 = 1000;
    auto expected3 = 2000;
    std::deque temps{ expected1, expected2, expected3 };
    std::queue queue(temps);

    // Exercise
    mock.set(queue);

    // Verify
    expect(expected1 == mock.read().value());
    expect(expected2 == mock.read().value());
    expect(expected3 == mock.read().value());
    expect(throws([&mock] { mock.read().value(); }));
  };
};
}  // namespace embed
