#include <deque>

#include <boost/ut.hpp>
#include <libhal/counter/mock.hpp>

namespace hal {
boost::ut::suite counter_mock_test = []() {
  using namespace boost::ut;

  "hal::mock::counter::uptime()"_test = []() {
    // Setup
    hal::mock::counter mock;
    constexpr hal::counter::uptime_t expected1{ frequency{ 1'000'000 }, 0 };
    hal::counter::uptime_t expected2{ frequency{ 8'000'000 }, 1 };
    hal::counter::uptime_t expected3{ frequency{ 48'000'000 }, 2 };
    std::deque samples{ expected1, expected2, expected3 };
    std::queue queue(samples);

    // Exercise + Verify
    mock.set(queue);
    auto result = mock.uptime().value();
    expect(expected1.count == result.count);
    expect(expected1.frequency == result.frequency);

    result = mock.uptime().value();
    expect(expected2.count == result.count);
    expect(expected2.frequency == result.frequency);

    result = mock.uptime().value();
    expect(expected3.count == result.count);
    expect(expected3.frequency == result.frequency);

    expect(throws([&mock] { mock.uptime().value(); }));
  };
};
}  // namespace hal
