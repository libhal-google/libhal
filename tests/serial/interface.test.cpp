#include <boost/ut.hpp>
#include <libhal/serial/interface.hpp>

namespace hal {
namespace {
constexpr hal::serial::settings expected_settings{
  .baud_rate = 115200.0f,
  .stop = hal::serial::settings::stop_bits::two,
  .parity = hal::serial::settings::parity::none
};

class test_serial : public hal::serial
{
public:
  settings m_settings{};
  bool m_flush_called{ false };
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

  result<write_t> driver_write(
    std::span<const hal::byte> p_data) noexcept override
  {
    if (m_return_error_status) {
      return hal::new_error();
    }
    return write_t{ p_data, std::span<const hal::byte>{} };
  };

  result<read_t> driver_read(std::span<hal::byte> p_data) noexcept override
  {
    if (m_return_error_status) {
      return hal::new_error();
    }
    return read_t{
      .received = p_data.subspan(0, 1),
      .remaining = p_data.subspan(1),
      .available = 1,
      .capacity = 1,
    };
  };

  status driver_flush() noexcept override
  {
    m_flush_called = true;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };
};
}  // namespace

boost::ut::suite serial_test = []() {
  using namespace boost::ut;
  "serial interface test"_test = []() {
    // Setup
    test_serial test;
    const std::array<hal::byte, 4> expected_payload{ 'a', 'b' };
    std::array<hal::byte, 4> expected_buffer{ '1', '2' };

    // Exercise
    auto result1 = test.configure(expected_settings);
    auto result2 = test.write(expected_payload);
    auto result3 = test.read(expected_buffer);
    auto result4 = test.flush();

    // Verify
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(bool{ result3 });
    expect(bool{ result4 });
    expect(expected_settings.baud_rate == test.m_settings.baud_rate);
    expect(expected_settings.stop == test.m_settings.stop);
    expect(expected_settings.parity == test.m_settings.parity);
    expect(that % expected_payload.data() ==
           result2.value().transmitted.data());
    expect(that % expected_buffer.data() == result3.value().received.data());
    expect(true == test.m_flush_called);
  };

  "serial errors test"_test = []() {
    // Setup
    test_serial test;
    const std::array<hal::byte, 4> expected_payload{ 'a', 'b' };
    std::array<hal::byte, 4> expected_buffer{ '1', '2' };
    test.m_return_error_status = true;

    // Exercise
    auto result1 = test.configure(expected_settings);
    auto result2 = test.write(expected_payload);
    auto result3 = test.read(expected_buffer);
    auto result4 = test.flush();

    // Verify
    expect(!bool{ result1 });
    expect(!bool{ result2 });
    expect(!bool{ result3 });
    expect(!bool{ result4 });
  };
};
}  // namespace hal
