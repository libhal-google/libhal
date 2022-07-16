#include <deque>

#include <boost/ut.hpp>
#include <libembeddedhal/accelerometer/mock.hpp>

namespace embed {
boost::ut::suite accelerometer_mock_test = []() {
  using namespace boost::ut;

  "embed::mock::accelerometer::read()"_test = []() {
    // Setup
    embed::mock::accelerometer mock;
    embed::accelerometer::sample expected1;
    expected1.axis.x = embed::percent::from_ratio(1, 4);
    expected1.axis.y = embed::percent::from_ratio(1, 4);
    expected1.axis.z = embed::percent::from_ratio(1, 4);
    embed::accelerometer::sample expected2;
    expected2.axis.x = embed::percent::from_ratio(1, 2);
    expected2.axis.y = embed::percent::from_ratio(1, 2);
    expected2.axis.z = embed::percent::from_ratio(1, 2);
    embed::accelerometer::sample expected3;
    expected3.axis.x = embed::percent::from_ratio(1, 1);
    expected3.axis.y = embed::percent::from_ratio(1, 1);
    expected3.axis.z = embed::percent::from_ratio(1, 1);

    std::deque samples{ expected1, expected2, expected3 };
    std::queue queue(samples);

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
