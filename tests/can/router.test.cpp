#include <libhal/can/router.hpp>

#include <algorithm>
#include <boost/ut.hpp>

namespace hal {
namespace {
class mock_can : public hal::can
{
public:
  settings m_settings{};
  message_t m_message{};
  std::function<handler> m_handler{};
  bool m_return_error_status{ false };

private:
  status driver_configure(const settings& p_settings) noexcept override
  {
    m_settings = p_settings;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };

  status driver_send(const message_t& p_message) noexcept override
  {
    m_message = p_message;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };

  status driver_on_receive(std::function<handler> p_handler) noexcept override
  {
    m_handler = p_handler;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };
};
}  // namespace

boost::ut::suite can_router_test = []() {
  using namespace boost::ut;

  "can_router::create + update_callback()"_test = []() {
    // Setup
    mock_can mock;

    // Exercise
    auto router = can_router::create(mock);

    // Verify
    expect(bool{ router });
  };

  "can_router::create failure"_test = []() {
    // Setup
    mock_can mock;
    mock.m_return_error_status = true;

    // Exercise
    auto router = can_router::create(mock);

    // Verify
    expect(!bool{ router });
  };

  "can_router::bus()"_test = []() {
    // Setup
    static constexpr can::message_t expected{
      .id = 0x111,
      .payload = { 0xAA, 0xBB, 0xCC },
      .length = 3,
    };
    mock_can mock;
    auto router = can_router::create(mock).value();

    // Exercise
    auto status = router.bus().send(expected);

    // Verify
    expect(bool{ status });
    expect(expected == mock.m_message);
  };

  "can_router::bus() failed"_test = []() {
    // Setup
    static constexpr can::message_t expected{
      .id = 0x111,
      .payload = { 0xAA, 0xBB, 0xCC },
      .length = 3,
    };
    mock_can mock;
    auto router = can_router::create(mock).value();
    mock.m_return_error_status = true;

    // Exercise
    auto status = router.bus().send(expected);

    // Verify
    expect(!bool{ status });
    expect(expected == mock.m_message);
  };

  "can_router::add_message_callback(id)"_test = []() {
    // Setup
    static constexpr can::id_t id = 0x15;
    mock_can mock;
    auto router = can_router::create(mock).value();

    // Exercise
    auto callback_item = router.add_message_callback(id);

    // Verify
    expect(that % id == callback_item.get().id);
    expect(that % 1 == router.handlers().size());

    [[maybe_unused]] const auto& iterator = std::find_if(
      router.handlers().begin(),
      router.handlers().cend(),
      [](const can_router::route& p_route) { return p_route.id == id; });
    expect(iterator->id == callback_item.get().id);
  };

  "can_router::add_message_callback(id, callback)"_test = []() {
    // Setup
    static constexpr can::id_t id = 0x15;
    static constexpr can::message_t expected{
      .id = 0x111,
      .payload = { 0xAA, 0xBB, 0xCC },
      .length = 3,
    };
    mock_can mock;
    auto router = can_router::create(mock).value();
    int counter = 0;
    can::message_t actual{};

    // Exercise
    auto callback_item = router.add_message_callback(
      id,
      [&counter, &actual]([[maybe_unused]] const can::message_t& p_message) {
        counter++;
        actual = p_message;
      });

    [[maybe_unused]] const auto& iterator = std::find_if(
      router.handlers().begin(),
      router.handlers().cend(),
      [](const can_router::route& p_route) { return p_route.id == id; });

    iterator->handler(expected);

    // Verify
    expect(that % id == callback_item.get().id);
    expect(that % 1 == router.handlers().size());
    expect(iterator->id == callback_item.get().id);
    expect(that % 1 == counter);
    expect(expected == actual);
  };

  "can_router::add_message_callback(id, callback)"_test = []() {
    // Setup
    mock_can mock;
    auto router = can_router::create(mock).value();
    int counter1 = 0;
    int counter2 = 0;
    int counter3 = 0;
    static constexpr can::message_t expected1{
      .id = 0x100,
      .payload = { 0xAA, 0xBB },
      .length = 2,
    };
    static constexpr can::message_t expected2{
      .id = 0x120,
      .payload = { 0xCC, 0xDD },
      .length = 2,
    };
    static constexpr can::message_t expected3{
      .id = 0x123,
      .payload = { 0xEE, 0xFF },
      .length = 2,
    };

    can::message_t actual1{};
    can::message_t actual2{};
    can::message_t actual3{};

    auto message_handler1 = router.add_message_callback(
      expected1.id,
      [&counter1, &actual1]([[maybe_unused]] const can::message_t& p_message) {
        counter1++;
        actual1 = p_message;
      });

    auto message_handler2 = router.add_message_callback(
      expected2.id,
      [&counter2, &actual2]([[maybe_unused]] const can::message_t& p_message) {
        counter2++;
        actual2 = p_message;
      });

    auto message_handler3 = router.add_message_callback(
      expected3.id,
      [&counter3, &actual3]([[maybe_unused]] const can::message_t& p_message) {
        counter3++;
        actual3 = p_message;
      });

    // Exercise
    router(expected1);

    // Verify
    expect(that % 3 == router.handlers().size());
    expect(that % 1 == counter1);
    expect(expected1 == actual1);
    expect(that % 0 == counter2);
    expect(expected2 != actual2);
    expect(that % 0 == counter2);
    expect(expected3 != actual3);

    router(expected2);

    expect(that % 1 == counter1);
    expect(expected1 == actual1);
    expect(that % 1 == counter2);
    expect(expected2 == actual2);
    expect(that % 0 == counter3);
    expect(expected3 != actual3);

    router(expected3);

    expect(that % 1 == counter1);
    expect(expected1 == actual1);
    expect(that % 1 == counter2);
    expect(expected2 == actual2);
    expect(that % 1 == counter2);
    expect(expected3 == actual3);

    router(expected2);

    expect(that % 1 == counter1);
    expect(expected1 == actual1);
    expect(that % 2 == counter2);
    expect(expected2 == actual2);
    expect(that % 1 == counter3);
    expect(expected3 == actual3);

    message_handler2.~item();
    expect(that % 2 == router.handlers().size());

    router(expected2);

    expect(that % 1 == counter1);
    expect(expected1 == actual1);
    expect(that % 2 == counter2);  // stays the same
    expect(expected2 == actual2);
    expect(that % 1 == counter3);
    expect(expected3 == actual3);

    router(expected3);

    expect(that % 1 == counter1);
    expect(expected1 == actual1);
    expect(that % 2 == counter2);
    expect(expected2 == actual2);
    expect(that % 2 == counter2);
    expect(expected3 == actual3);
  };
};
}  // namespace hal
