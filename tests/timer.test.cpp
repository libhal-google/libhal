#include <boost/ut.hpp>
#include <libhal/timer.hpp>

namespace hal {
namespace {
class test_timer : public hal::timer
{
public:
  bool m_is_running{ false };
  std::function<void(void)> m_callback{};
  hal::time_duration m_delay;
  bool m_return_error_status{ false };

private:
  result<bool> driver_is_running() noexcept override
  {
    if (m_return_error_status) {
      return hal::new_error();
    }
    return m_is_running;
  };
  status driver_cancel() noexcept override
  {
    m_is_running = false;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };
  status driver_schedule(std::function<void(void)> p_callback,
                         hal::time_duration p_delay) noexcept override
  {
    m_is_running = true;
    m_callback = p_callback;
    m_delay = p_delay;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };
};
}  // namespace

boost::ut::suite timer_test = []() {
  using namespace boost::ut;
  "timer interface test"_test = []() {
    // Setup
    test_timer test;
    const std::function<void(void)> expected_callback = []() {};
    const std::chrono::nanoseconds expected_delay = {};

    // Exercise + Verify
    auto result1 = test.is_running();
    expect(bool{ result1 });
    expect(that % false == result1.value());

    auto result2 = test.schedule(expected_callback, expected_delay);
    result1 = test.is_running();
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(that % true == result1.value());
    expect(bool{ test.m_callback });
    expect(expected_delay == test.m_delay);

    auto result3 = test.cancel();
    result1 = test.is_running();
    expect(bool{ result1 });
    expect(bool{ result3 });
    expect(that % false == result1.value());
  };
  "timer errors test"_test = []() {
    // Setup
    test_timer test;
    const std::function<void(void)> expected_callback = []() {};
    const std::chrono::nanoseconds expected_delay = {};
    test.m_return_error_status = true;

    // Exercise
    auto result1 = test.is_running();
    auto result2 = test.schedule(expected_callback, expected_delay);
    auto result3 = test.cancel();

    // Verify
    expect(!bool{ result1 });
    expect(!bool{ result2 });
    expect(!bool{ result3 });
  };
};
}  // namespace hal
