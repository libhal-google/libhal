#include <boost/ut.hpp>
#include <libhal/can/mock.hpp>

namespace hal {
boost::ut::suite can_mock_test = []() {
  using namespace boost::ut;
  "hal::can::configure tests"_test = []() {
    // Setup
    hal::mock::can mock;
    constexpr hal::can::settings expected1{};
    constexpr hal::can::settings expected2{
      .baud_rate = 1.0_Hz,
    };
    mock.spy_configure.trigger_error_on_call(3);

    // Exercise + Verify
    expect(bool{ mock.configure(expected1) });
    expect(expected1 == std::get<0>(mock.spy_configure.call_history().at(0)));

    expect(bool{ mock.configure(expected2) });
    expect(expected2 == std::get<0>(mock.spy_configure.call_history().at(1)));

    expect(!mock.configure(expected2));
    expect(expected2 == std::get<0>(mock.spy_configure.call_history().at(2)));
  };

  "hal::can::send tests"_test = []() {
    // Setup
    hal::mock::can mock;
    constexpr hal::can::message_t expected1{ .id = 1,
                                             .payload = { 'a' },
                                             .length = 1 };
    constexpr hal::can::message_t expected2{ .id = 2,
                                             .payload = { 'b' },
                                             .length = 1 };
    mock.spy_send.trigger_error_on_call(3);

    // Exercise + Verify
    expect(bool{ mock.send(expected1) });
    expect(that % expected1.payload ==
           std::get<0>(mock.spy_send.call_history().at(0)).payload);
    expect(that % expected1.id ==
           std::get<0>(mock.spy_send.call_history().at(0)).id);

    expect(bool{ mock.send(expected2) });
    expect(that % expected2.payload ==
           std::get<0>(mock.spy_send.call_history().at(1)).payload);
    expect(that % expected2.id ==
           std::get<0>(mock.spy_send.call_history().at(1)).id);

    expect(!mock.send(expected2));
    expect(that % expected2.payload ==
           std::get<0>(mock.spy_send.call_history().at(2)).payload);
    expect(that % expected2.id ==
           std::get<0>(mock.spy_send.call_history().at(2)).id);
  };

  "hal::can::on_receive tests"_test = []() {
    // Setup
    hal::mock::can mock;
    int counter = 0;
    constexpr hal::can::message_t expected_message{ .id = 1,
                                                    .payload = { 'a' },
                                                    .length = 1 };
    const std::function<hal::can::handler> expected1 =
      [&counter](hal::can::message_t expected_message) {
        counter++;
        return expected_message;
      };
    const std::function<hal::can::handler> expected2 =
      [&counter](hal::can::message_t expected_message) {
        counter--;
        return expected_message;
      };
    mock.spy_on_receive.trigger_error_on_call(3);

    // Exercise + Verify
    expect(bool{ mock.on_receive(expected1) });
    std::function<hal::can::handler> handler1 =
      std::get<0>(mock.spy_on_receive.call_history().at(0));
    handler1(expected_message);
    expect(that % 1 == counter);

    expect(bool{ mock.on_receive(expected2) });
    std::function<hal::can::handler> handler2 =
      std::get<0>(mock.spy_on_receive.call_history().at(1));
    handler2(expected_message);
    expect(that % 0 == counter);

    expect(!mock.on_receive(expected2));
    std::function<hal::can::handler> handler3 =
      std::get<0>(mock.spy_on_receive.call_history().at(2));
    handler3(expected_message);
    expect(that % -1 == counter);
  };
};
}  // namespace hal
