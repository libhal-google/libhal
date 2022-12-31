#include <libhal/spi.hpp>

#include <boost/ut.hpp>

namespace hal {
namespace {
constexpr hal::spi::settings expected_settings{
  .clock_rate = 10.0_kHz,
  .clock_idles_high = true,
  .data_valid_on_trailing_edge = true,
};
class test_spi : public hal::spi
{
public:
  settings m_settings{};
  std::span<const hal::byte> m_data_out{};
  std::span<hal::byte> m_data_in{};
  hal::byte m_filler{};
  bool m_return_error_status{ false };

  ~test_spi()
  {
  }

private:
  status driver_configure(const settings& p_settings) override
  {
    m_settings = p_settings;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };

  status driver_transfer(std::span<const hal::byte> p_data_out,
                         std::span<hal::byte> p_data_in,
                         hal::byte p_filler) override
  {
    m_data_out = p_data_out;
    m_data_in = p_data_in;
    m_filler = p_filler;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };
};
}  // namespace

void spi_test()
{
  using namespace boost::ut;
  "spi interface test"_test = []() {
    // Setup
    test_spi test;
    const std::array<hal::byte, 4> expected_out{ 'a', 'b' };
    std::array<hal::byte, 4> expected_in{ '1', '2' };
    const auto expected_filler = ' ';

    // Exercise
    auto result1 = test.configure(expected_settings);
    auto result2 = test.transfer(expected_out, expected_in, expected_filler);

    // Verify
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(that % expected_out.data() == test.m_data_out.data());
    expect(that % expected_in.data() == test.m_data_in.data());
    expect(expected_filler == test.m_filler);
  };

  "spi errors test"_test = []() {
    // Setup
    test_spi test;
    const std::array<hal::byte, 4> expected_out{ 'a', 'b' };
    std::array<hal::byte, 4> expected_in{ '1', '2' };
    const auto expected_filler = ' ';
    test.m_return_error_status = true;

    // Exercise
    auto result1 = test.configure(expected_settings);
    auto result2 = test.transfer(expected_out, expected_in, expected_filler);

    // Verify
    expect(!bool{ result1 });
    expect(!bool{ result2 });
  };
};
}  // namespace hal
