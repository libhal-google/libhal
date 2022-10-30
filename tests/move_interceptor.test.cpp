#include <libhal/move_interceptor.hpp>

#include <boost/ut.hpp>

namespace hal {
boost::ut::suite move_interceptor_test = []() {
  using namespace boost::ut;

  "move_interceptor<mock>::intercept"_test = []() {
    struct mock : public move_interceptor<mock>
    {
      mock() = default;
      mock(mock&) = delete;
      mock& operator=(mock&) = delete;
      mock& operator=(mock&& p_old_self) = default;
      mock(mock&& p_old_self) = default;

      void intercept(mock* p_old_self)
      {
        p_old_self->count++;
      }

      bool* intercept_was_called_first_ptr;
      int count = 0;
    };

    // Setup
    mock mock1;

    // Exercise
    auto mock2 = std::move(mock1);
    // Verify
    expect(that % 1 == mock2.count);

    // Exercise
    auto mock3 = std::move(mock2);
    // Verify
    expect(that % 2 == mock3.count);

    // Exercise
    auto mock4 = std::move(mock3);
    // Verify
    expect(that % 3 == mock4.count);

    // Exercise
    auto mock5 = std::move(mock4);
    // Verify
    expect(that % 4 == mock5.count);
  };
};
}  // namespace hal
