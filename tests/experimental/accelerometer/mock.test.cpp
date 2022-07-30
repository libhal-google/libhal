#include <deque>

#include <boost/ut.hpp>
#include <libembeddedhal/accelerometer/mock.hpp>

namespace hal {
boost::ut::suite accelerometer_mock_test = []() {
  using namespace boost::ut;

  "hal::mock::accelerometer::read()"_test = []() {
    // Setup
    hal::mock::accelerometer mock;
    hal::accelerometer::sample expected1;
    expected1.axis.x = hal::percent::from_ratio(1, 4);
    expected1.axis.y = hal::percent::from_ratio(1, 4);
    expected1.axis.z = hal::percent::from_ratio(1, 4);
    hal::accelerometer::sample expected2;
    expected2.axis.x = hal::percent::from_ratio(1, 2);
    expected2.axis.y = hal::percent::from_ratio(1, 2);
    expected2.axis.z = hal::percent::from_ratio(1, 2);
    hal::accelerometer::sample expected3;
    expected3.axis.x = hal::percent::from_ratio(1, 1);
    expected3.axis.y = hal::percent::from_ratio(1, 1);
    expected3.axis.z = hal::percent::from_ratio(1, 1);

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
}  // namespace hal
