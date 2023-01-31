#include <libhal/can.hpp>

#include <functional>

#include <boost/ut.hpp>

namespace hal {
namespace {
constexpr hal::can::settings expected_settings{
  .baud_rate = 1.0_Hz,
};
int counter = 0;
constexpr hal::can::message_t expected_message{ .id = 1, .length = 0 };
hal::callback<hal::can::handler> expected_handler =
  [](const hal::can::message_t&) { counter++; };

class test_can : public hal::can
{
public:
  settings m_settings{};
  message_t m_message{};
  hal::callback<handler> m_handler = [](const message_t&) {};
  bool m_return_error_status{ false };

private:
  status driver_configure(const settings& p_settings) override
  {
    m_settings = p_settings;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };

  status driver_send(const message_t& p_message) override
  {
    m_message = p_message;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };

  void driver_on_receive(hal::callback<handler> p_handler) override
  {
    m_handler = p_handler;
  };
};
}  // namespace

void can_test()
{
  using namespace boost::ut;

  "can interface test"_test = []() {
    // Setup
    test_can test;

    // Exercise
    auto result1 = test.configure(expected_settings);
    auto result2 = test.send(expected_message);
    test.on_receive(expected_handler);
    test.m_handler(expected_message);

    // Verify
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(that % expected_settings.baud_rate == test.m_settings.baud_rate);
    expect(that % expected_message.id == test.m_message.id);
    expect(that % 1 == counter);
  };

  "can errors test"_test = []() {
    // Setup
    test_can test;
    test.m_return_error_status = true;

    // Exercise
    auto result1 = test.configure(expected_settings);
    auto result2 = test.send(expected_message);

    // Verify
    expect(!bool{ result1 });
    expect(!bool{ result2 });
  };
};
}  // namespace hal
