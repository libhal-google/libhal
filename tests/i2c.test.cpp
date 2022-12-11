#include <boost/ut.hpp>
#include <libhal/i2c.hpp>

namespace hal {
namespace {
constexpr hal::i2c::settings expected_settings{ .clock_rate = 1.0_Hz };
constexpr hal::byte expected_address{ 100 };
constexpr std::array<hal::byte, 4> expected_data_out{ 'a', 'b' };
std::array<hal::byte, 4> expected_data_in{ '1', '2' };
const std::function<hal::timeout_function> expected_timeout = []() {
  return success();
};

class test_i2c : public hal::i2c
{
public:
  settings m_settings{};
  hal::byte m_address{};
  std::span<const hal::byte> m_data_out{};
  std::span<hal::byte> m_data_in{};
  std::function<hal::timeout_function> m_timeout{};
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
  status driver_transaction(
    hal::byte p_address,
    std::span<const hal::byte> p_data_out,
    std::span<hal::byte> p_data_in,
    std::function<hal::timeout_function> p_timeout) noexcept override
  {
    HAL_CHECK(p_timeout());
    m_address = p_address;
    m_data_out = p_data_out;
    m_data_in = p_data_in;
    m_timeout = p_timeout;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };
};
}  // namespace

void i2c_test()
{
  using namespace boost::ut;
  "i2c interface test"_test = []() {
    // Setup
    test_i2c test;

    // Exercise
    auto result1 = test.configure(expected_settings);
    auto result2 = test.transaction(
      expected_address, expected_data_out, expected_data_in, expected_timeout);

    // Verify
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(that % expected_settings.clock_rate == test.m_settings.clock_rate);
    expect(that % expected_address == test.m_address);
    expect(that % expected_data_out.data() == test.m_data_out.data());
    expect(that % expected_data_in.data() == test.m_data_in.data());
  };

  "i2c errors test"_test = []() {
    // Setup
    test_i2c test;
    test.m_return_error_status = true;

    // Exercise
    auto result1 = test.configure(expected_settings);
    auto result2 = test.transaction(
      expected_address, expected_data_out, expected_data_in, expected_timeout);

    // Verify
    expect(!bool{ result1 });
    expect(!bool{ result2 });
  };
};
}  // namespace hal
