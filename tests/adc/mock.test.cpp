#include <deque>

#include <boost/ut.hpp>
#include <libhal/adc/mock.hpp>

namespace hal {
boost::ut::suite adc_mock_test = []() {
  using namespace boost::ut;

  // Setup
  auto expected1 = float(0.25);
  auto expected2 = float(0.5);
  auto expected3 = float(1.0);
  hal::mock::adc mock;
  std::deque percentages{ expected1, expected2, expected3 };
  std::queue queue(percentages);

  // Exercise
  mock.set(queue);
  auto result1 = mock.read().value();
  auto result2 = mock.read().value();
  auto result3 = mock.read().value();

  // Verify
  expect(that % expected1 == result1);
  expect(that % expected2 == result2);
  expect(that % expected3 == result3);
};
}  // namespace hal
