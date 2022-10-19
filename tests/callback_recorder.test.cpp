#include <libhal/callback_recorder.hpp>

#include <boost/ut.hpp>

namespace hal {
boost::ut::suite callback_recorder_test = []() {
  using namespace boost::ut;

  "callback_recorder::ctor(callback_function)"_test = []() {
    // Setup
    int counter = 0;
    const std::function<void(void)> expected_callback = [&counter](void) {
      counter++;
    };
    hal::mock::callback_recorder<void(void)> recorder(expected_callback);

    // Exercise + Verify
    auto recorded_callback = recorder.callback();
    expect(that % 0 == counter);
    expect(that % 0 == recorder.call_count());
    expect(that % false == recorder.was_called());
    expect(that % false == recorder.was_called_once());

    recorded_callback();
    expect(that % 1 == counter);
    expect(that % 1 == recorder.call_count());
    expect(that % true == recorder.was_called());
    expect(that % true == recorder.was_called_once());
    expect(that % true == recorder.was_called_n_times(1));
    recorded_callback();
    expect(that % false == recorder.was_called_once());
    expect(that % true == recorder.was_called_n_times(2));

    recorder.clear_call_count();
    expect(that % 0 == recorder.call_count());
    expect(that % false == recorder.was_called());
    expect(that % false == recorder.was_called_once());

    recorded_callback();
    expect(that % 3 == counter);
    expect(that % 1 == recorder.call_count());
    expect(that % true == recorder.was_called());
    expect(that % true == recorder.was_called_once());
  };

  "callback_recorder::ctor(void)"_test = []() {
    // Setup
    hal::mock::callback_recorder<void(void)> recorder;

    // Exercise + Verify
    auto recorded_callback = recorder.callback();
    expect(that % 0 == recorder.call_count());
    expect(that % false == recorder.was_called());
    expect(that % false == recorder.was_called_once());

    recorded_callback();
    expect(that % 1 == recorder.call_count());
    expect(that % true == recorder.was_called());
    expect(that % true == recorder.was_called_once());
    expect(that % true == recorder.was_called_n_times(1));
    recorded_callback();
    expect(that % false == recorder.was_called_once());
    expect(that % true == recorder.was_called_n_times(2));

    recorder.clear_call_count();
    expect(that % 0 == recorder.call_count());
    expect(that % false == recorder.was_called());
    expect(that % false == recorder.was_called_once());
  };

  "callback_recorder::ctor(default)"_test = []() {
    // Setup
    hal::mock::callback_recorder<int(void)> recorder(5);

    // Exercise + Verify
    auto recorded_callback = recorder.callback();
    expect(that % 0 == recorder.call_count());
    expect(that % false == recorder.was_called());
    expect(that % false == recorder.was_called_once());

    auto result = recorded_callback();
    expect(that % 5 == result);
    expect(that % 1 == recorder.call_count());
    expect(that % true == recorder.was_called());
    expect(that % true == recorder.was_called_once());
    expect(that % true == recorder.was_called_n_times(1));

    recorder.clear_call_count();
    expect(that % 0 == recorder.call_count());
    expect(that % false == recorder.was_called());
    expect(that % false == recorder.was_called_once());
  };
};
}  // namespace hal
