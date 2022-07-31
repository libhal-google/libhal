#include <boost/ut.hpp>
#include <libhal/pwm/mock.hpp>

namespace hal {
boost::ut::suite testing_utilities_test = []() {
  using namespace boost::ut;

  spy_handler<int, char> spy;

  expect(that % 0 == spy.call_history().size());

  spy.trigger_error_on_call(4);

  expect(bool{ spy.record(1, 'A') });
  expect(that % 1 == spy.call_history().size());
  expect(that % 1 == std::get<0>(spy.call_history().at(0)));
  expect(that % 'A' == std::get<1>(spy.call_history().at(0)));

  expect(bool{ spy.record(2, 'B') });
  expect(that % 2 == spy.call_history().size());
  expect(that % 1 == std::get<0>(spy.call_history().at(0)));
  expect(that % 'A' == std::get<1>(spy.call_history().at(0)));
  expect(that % 2 == std::get<0>(spy.call_history().at(1)));
  expect(that % 'B' == std::get<1>(spy.call_history().at(1)));

  expect(bool{ spy.record(3, 'C') });
  expect(that % 3 == spy.call_history().size());
  expect(that % 1 == std::get<0>(spy.call_history().at(0)));
  expect(that % 'A' == std::get<1>(spy.call_history().at(0)));
  expect(that % 2 == std::get<0>(spy.call_history().at(1)));
  expect(that % 'B' == std::get<1>(spy.call_history().at(1)));
  expect(that % 3 == std::get<0>(spy.call_history().at(2)));
  expect(that % 'C' == std::get<1>(spy.call_history().at(2)));

  expect(!spy.record(4, 'D'));
  expect(that % 4 == spy.call_history().size());
  expect(that % 1 == std::get<0>(spy.call_history().at(0)));
  expect(that % 'A' == std::get<1>(spy.call_history().at(0)));
  expect(that % 2 == std::get<0>(spy.call_history().at(1)));
  expect(that % 'B' == std::get<1>(spy.call_history().at(1)));
  expect(that % 3 == std::get<0>(spy.call_history().at(2)));
  expect(that % 'C' == std::get<1>(spy.call_history().at(2)));
  expect(that % 4 == std::get<0>(spy.call_history().at(3)));
  expect(that % 'D' == std::get<1>(spy.call_history().at(3)));

  spy.reset();

  expect(that % 0 == spy.call_history().size());

  expect(bool{ spy.record(1, 'A') });
  expect(that % 1 == spy.call_history().size());
  expect(that % 1 == std::get<0>(spy.call_history().at(0)));
  expect(that % 'A' == std::get<1>(spy.call_history().at(0)));

  expect(bool{ spy.record(2, 'B') });
  expect(that % 2 == spy.call_history().size());
  expect(that % 1 == std::get<0>(spy.call_history().at(0)));
  expect(that % 'A' == std::get<1>(spy.call_history().at(0)));
  expect(that % 2 == std::get<0>(spy.call_history().at(1)));
  expect(that % 'B' == std::get<1>(spy.call_history().at(1)));

  expect(bool{ spy.record(3, 'C') });
  expect(that % 3 == spy.call_history().size());
  expect(that % 1 == std::get<0>(spy.call_history().at(0)));
  expect(that % 'A' == std::get<1>(spy.call_history().at(0)));
  expect(that % 2 == std::get<0>(spy.call_history().at(1)));
  expect(that % 'B' == std::get<1>(spy.call_history().at(1)));
  expect(that % 3 == std::get<0>(spy.call_history().at(2)));
  expect(that % 'C' == std::get<1>(spy.call_history().at(2)));

  expect(bool{ spy.record(4, 'D') });
  expect(that % 4 == spy.call_history().size());
  expect(that % 1 == std::get<0>(spy.call_history().at(0)));
  expect(that % 'A' == std::get<1>(spy.call_history().at(0)));
  expect(that % 2 == std::get<0>(spy.call_history().at(1)));
  expect(that % 'B' == std::get<1>(spy.call_history().at(1)));
  expect(that % 3 == std::get<0>(spy.call_history().at(2)));
  expect(that % 'C' == std::get<1>(spy.call_history().at(2)));
  expect(that % 4 == std::get<0>(spy.call_history().at(3)));
  expect(that % 'D' == std::get<1>(spy.call_history().at(3)));
};
}  // namespace hal
