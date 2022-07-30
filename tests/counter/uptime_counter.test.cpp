#include <boost/ut.hpp>
#include <libembeddedhal/counter/uptime_counter.hpp>
#include <queue>

namespace hal {

boost::ut::suite uptime_utility_test = []() {
  using namespace boost::ut;
  using namespace std::literals;
  using namespace hal::literals;

  class mock_counter : public hal::counter
  {
  public:
    boost::leaf::result<uptime_t> driver_uptime() noexcept override
    {
      auto count = uptime_sequence.front();
      uptime_sequence.pop();
      return uptime_t{ m_frequency, count };
    };

    std::queue<std::uint32_t> uptime_sequence{};

  private:
    hal::frequency m_frequency{ 1'000_MHz };
  };

  "[uptime_counter] zero"_test = []() {
    // Setup
    mock_counter mock;
    uptime_counter uptime(mock);
    mock.uptime_sequence.push(0);
    mock.uptime_sequence.push(0);
    mock.uptime_sequence.push(0);
    mock.uptime_sequence.push(0);

    // Exercise
    auto nanoseconds0 = uptime.uptime().value();
    auto nanoseconds1 = uptime.uptime().value();
    auto nanoseconds2 = uptime.uptime().value();
    auto nanoseconds3 = uptime.uptime().value();

    // Verify
    expect(that % (0ns).count() == nanoseconds0.count());
    expect(that % (0ns).count() == nanoseconds1.count());
    expect(that % (0ns).count() == nanoseconds2.count());
    expect(that % (0ns).count() == nanoseconds3.count());
  };

  "[uptime_counter] one"_test = []() {
    // Setup
    mock_counter mock;
    uptime_counter uptime(mock);
    mock.uptime_sequence.push(1);
    mock.uptime_sequence.push(2);
    mock.uptime_sequence.push(3);
    mock.uptime_sequence.push(4);

    // Exercise
    auto nanoseconds0 = uptime.uptime().value();
    auto nanoseconds1 = uptime.uptime().value();
    auto nanoseconds2 = uptime.uptime().value();
    auto nanoseconds3 = uptime.uptime().value();

    // Verify
    expect(that % (1ns).count() == nanoseconds0.count());
    expect(that % (2ns).count() == nanoseconds1.count());
    expect(that % (3ns).count() == nanoseconds2.count());
    expect(that % (4ns).count() == nanoseconds3.count());
  };

  "[uptime_counter] many"_test = []() {
    // Setup
    mock_counter mock;
    uptime_counter uptime(mock);
    mock.uptime_sequence.push(1);
    mock.uptime_sequence.push(20);
    mock.uptime_sequence.push(300);
    mock.uptime_sequence.push(4000);
    mock.uptime_sequence.push(50000);
    mock.uptime_sequence.push(600000);
    mock.uptime_sequence.push(7000000);
    mock.uptime_sequence.push(7000001);
    mock.uptime_sequence.push(7000001);
    mock.uptime_sequence.push(7000005);

    // Exercise
    auto nanoseconds0 = uptime.uptime().value();
    auto nanoseconds1 = uptime.uptime().value();
    auto nanoseconds2 = uptime.uptime().value();
    auto nanoseconds3 = uptime.uptime().value();
    auto nanoseconds4 = uptime.uptime().value();
    auto nanoseconds5 = uptime.uptime().value();
    auto nanoseconds6 = uptime.uptime().value();
    auto nanoseconds7 = uptime.uptime().value();
    auto nanoseconds8 = uptime.uptime().value();
    auto nanoseconds9 = uptime.uptime().value();

    // Verify
    expect(that % (1ns).count() == nanoseconds0.count());
    expect(that % (20ns).count() == nanoseconds1.count());
    expect(that % (300ns).count() == nanoseconds2.count());
    expect(that % (4000ns).count() == nanoseconds3.count());
    expect(that % (50000ns).count() == nanoseconds4.count());
    expect(that % (600000ns).count() == nanoseconds5.count());
    expect(that % (7000000ns).count() == nanoseconds6.count());
    expect(that % (7000001ns).count() == nanoseconds7.count());
    expect(that % (7000001ns).count() == nanoseconds8.count());
    expect(that % (7000005ns).count() == nanoseconds9.count());
  };

  "[uptime_counter] boundaries"_test = []() {
    // Setup
    mock_counter mock;
    uptime_counter uptime(mock);
    constexpr auto max = std::numeric_limits<uint32_t>::max();
    constexpr auto max_ns = std::chrono::nanoseconds(max);
    constexpr auto d_max = max_ns * 2;

    mock.uptime_sequence.push(1);
    mock.uptime_sequence.push(100);
    mock.uptime_sequence.push(max);
    mock.uptime_sequence.push(50'000);
    mock.uptime_sequence.push(max);
    mock.uptime_sequence.push(12'345);

    // Exercise
    auto nanoseconds0 = uptime.uptime().value();
    auto nanoseconds1 = uptime.uptime().value();
    auto nanoseconds2 = uptime.uptime().value();
    auto nanoseconds3 = uptime.uptime().value();
    auto nanoseconds4 = uptime.uptime().value();
    auto nanoseconds5 = uptime.uptime().value();

    // Verify
    expect(that % (1ns).count() == nanoseconds0.count());
    expect(that % (100ns).count() == nanoseconds1.count());
    expect(that % max_ns.count() == nanoseconds2.count());
    expect(that % (max_ns + 50'000ns + 1ns).count() == nanoseconds3.count());
    expect(that % (d_max + 1ns).count() == nanoseconds4.count());
    expect(that % (d_max + 12'345ns + 2ns).count() == nanoseconds5.count());
  };

  "[uptime_counter] errors"_test = []() {
    // None at the moment
  };
};
}  // namespace hal
