#include <deque>

#include <boost/ut.hpp>
#include <libhal/steady_clock/mock.hpp>

namespace hal {
boost::ut::suite steady_clock_mock_test = []() {
  using namespace boost::ut;

  // Setup
  std::uint64_t expected1 = 10;
  std::uint64_t expected2 = 50;
  std::uint64_t expected3 = 100;
  auto expected_frequency = 1.0_Hz;
  hal::mock::steady_clock mock;
  std::deque uptimes{ expected1, expected2, expected3 };
  std::queue queue(uptimes);

  // Exercise
  mock.set_frequency(expected_frequency);
  mock.set_uptimes(queue);
  auto result0 = mock.frequency();
  auto result1 = mock.uptime().value();
  auto result2 = mock.uptime().value();
  auto result3 = mock.uptime().value();

  // Verify
  expect(expected_frequency = result0);
  expect(that % expected1 == result1);
  expect(that % expected2 == result2);
  expect(that % expected3 == result3);
  expect(!bool{ mock.uptime() });
};
}  // namespace hal
