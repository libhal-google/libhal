#include <boost/ut.hpp>
#include <libhal/dac.hpp>

namespace hal {
namespace {
constexpr auto expected_value = float(0.5);
class test_dac : public hal::dac
{
public:
  float m_passed_value{};
  bool m_return_error_status{ false };

  ~test_dac()
  {
  }

private:
  status driver_write(float p_value) noexcept override
  {
    m_passed_value = p_value;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  }
};
}  // namespace

void dac_test()
{
  using namespace boost::ut;

  "dac interface test"_test = []() {
    using namespace boost::ut;
    // Setup
    test_dac test;

    // Exercise
    auto result = test.write(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % expected_value == test.m_passed_value);
  };

  "dac errors test"_test = []() {
    // Setup
    test_dac test;
    test.m_return_error_status = true;

    // Exercise
    auto result = test.write(expected_value);

    // Verify
    expect(!bool{ result });
  };
};
}  // namespace hal
