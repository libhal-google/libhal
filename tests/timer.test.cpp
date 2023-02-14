#include <libhal/timer.hpp>

#include <functional>

#include <boost/ut.hpp>

namespace hal {
namespace {
class test_timer : public hal::timer
{
public:
  bool m_is_running{ false };
  hal::callback<void(void)> m_callback = []() {};
  hal::time_duration m_delay;
  bool m_return_error_status{ false };

private:
  result<is_running_t> driver_is_running() override
  {
    if (m_return_error_status) {
      return hal::new_error();
    }
    return is_running_t{ .is_running = m_is_running };
  };
  result<cancel_t> driver_cancel() override
  {
    m_is_running = false;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return cancel_t{};
  };
  result<schedule_t> driver_schedule(hal::callback<void(void)> p_callback,
                                     hal::time_duration p_delay) override
  {
    m_is_running = true;
    m_callback = p_callback;
    m_delay = p_delay;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return schedule_t{};
  };
};
}  // namespace

void timer_test()
{
  using namespace boost::ut;
  "timer interface test"_test = []() {
    // Setup
    test_timer test;
    bool callback_stored_successfully = false;
    const hal::callback<void(void)> expected_callback =
      [&callback_stored_successfully]() {
        callback_stored_successfully = true;
      };
    const hal::time_duration expected_delay = {};

    // Exercise + Verify
    auto result1 = test.is_running();
    expect(bool{ result1 });
    expect(that % false == result1.value().is_running);

    auto result2 = test.schedule(expected_callback, expected_delay);
    result1 = test.is_running();
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(that % true == result1.value().is_running);
    expect(expected_delay == test.m_delay);

    test.m_callback();
    expect(that % true == callback_stored_successfully);

    auto result3 = test.cancel();
    result1 = test.is_running();
    expect(bool{ result1 });
    expect(bool{ result3 });
    expect(that % false == result1.value().is_running);
  };
  "timer errors test"_test = []() {
    // Setup
    test_timer test;
    const hal::function_ref<void(void)> expected_callback = []() {};
    const hal::time_duration expected_delay = {};
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
