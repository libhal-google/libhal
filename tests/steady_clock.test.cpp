#include <libhal/steady_clock.hpp>

#include <boost/ut.hpp>

namespace hal {
namespace {
class test_steady_clock : public hal::steady_clock
{
public:
  constexpr static hertz m_frequency{ 1.0_Hz };
  constexpr static std::uint64_t m_uptime{ 100 };
  bool m_return_error_status{ false };

  ~test_steady_clock()
  {
  }

private:
  result<frequency_t> driver_frequency() override
  {
    return frequency_t{ .operating_frequency = m_frequency };
  };

  result<uptime_t> driver_uptime() override
  {
    if (m_return_error_status) {
      return hal::new_error();
    }
    return uptime_t{ .ticks = m_uptime };
  };
};
}  // namespace

void steady_clock_test()
{
  using namespace boost::ut;
  "steady_clock interface test"_test = []() {
    // Setup
    test_steady_clock test;

    // Exercise
    auto result1 = test.frequency();
    auto result2 = test.uptime();

    // Verify
    expect(that % test.m_frequency == result1.value().operating_frequency);
    expect(bool{ result2 });
    expect(that % test.m_uptime == result2.value().ticks);
  };

  "steady_clock errors test"_test = []() {
    // Setup
    test_steady_clock test;
    test.m_return_error_status = true;

    // Exercise
    auto result = test.uptime();

    // Verify
    expect(!bool{ result });
  };
};
}  // namespace hal
