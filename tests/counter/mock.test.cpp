#include <deque>

#include <boost/ut.hpp>
#include <libembeddedhal/counter/mock.hpp>

namespace embed {
boost::ut::suite counter_mock_test = []() {
  using namespace boost::ut;

  "embed::mock::counter::uptime()"_test = []() {
    // Setup
    embed::mock::counter mock;
    constexpr embed::counter_interface::uptime_t expected1{
      frequency{ 1'000'000 }, 0
    };
    embed::counter_interface::uptime_t expected2{ frequency{ 8'000'000 }, 1 };
    embed::counter_interface::uptime_t expected3{ frequency{ 48'000'000 }, 2 };
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
}  // namespace embed
