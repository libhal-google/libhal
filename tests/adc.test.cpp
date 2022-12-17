#include <libhal/adc.hpp>

#include <boost/ut.hpp>

namespace hal {
namespace {
constexpr auto expected_value = float(0.5);

class test_adc : public hal::adc
{
public:
  constexpr static float m_returned_position{ 0.5f };
  bool m_return_error_status{ false };

  ~test_adc()
  {
  }

private:
  result<float> driver_read() noexcept override
  {
    if (m_return_error_status) {
      return hal::new_error();
    }
    return m_returned_position;
  }
};
}  // namespace

void adc_test()
{
  using namespace boost::ut;
  "adc interface test"_test = []() {
    // Setup
    test_adc test;

    // Exercise
    auto result = test.read();

    // Verify
    expect(bool{ result });
    expect(that % expected_value == result.value());
  };

  "adc errors test"_test = []() {
    // Setup
    test_adc test;
    test.m_return_error_status = true;

    // Exercise
    auto result = test.read();

    // Verify
    expect(!bool{ result });
  };
}
}  // namespace hal
