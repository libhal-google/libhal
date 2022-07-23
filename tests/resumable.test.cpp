#include <libhal/resumable.hpp>

#include <boost/ut.hpp>

#include <array>

namespace hal {
boost::ut::suite resumable_test = []() {
  using namespace boost::ut;

  "hal::resumable<T>()"_test = []() {
    "hal::resumable<int>()"_test = []() {
      // Setup
      auto resumable_lambda = []() -> hal::resumable<int> {
        co_yield 1;
        co_yield 2;
        co_return;
      };

      // Exercise
      auto resumer = resumable_lambda();
      auto resumer_state1 = bool{ resumer };
      auto value1 = resumer();
      auto value2 = resumer();
      auto resumer_state2 = bool{ resumer };
      auto value3 = resumer();
      auto resumer_state3 = bool{ resumer };
      auto value4 = resumer();
      auto resumer_state4 = bool{ resumer };

      // Verify
      expect(that % resumer_state1);
      expect(that % 1 == value1);
      expect(that % 2 == value2);
      expect(that % resumer_state2);
      expect(that % 2 == value3);
      expect(that % 2 == value4);
      expect(that % !resumer_state3);
      expect(that % !resumer_state4);
    };
  };
};
}