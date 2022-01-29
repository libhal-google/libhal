#include <boost/ut.hpp>
#include <iosfwd>
#include <libembeddedhal/i2c/i2c_mock.hpp>
#include <libembeddedhal/i2c/i2c_util.hpp>
#include <span>

namespace {
std::ostream& operator<<(std::ostream& os, const std::byte& p_data)
{
  return (os << "std::byte{ " << std::to_integer<int>(p_data) << " }");
}
}  // namespace

namespace embed {
boost::ut::suite i2c_mock_test = []() {
  using namespace boost::ut;

  static constexpr std::byte device{ 0x15 };
  static constexpr std::byte control{ 0x02 };
  static constexpr std::byte calibration_register_address{ 0x03 };

  "[success] write"_test = []() {
    // Setup
    mock::i2c i2c;
    constexpr std::byte expected_value{ 0x55 };
    std::byte actual_value{ 0x00 };
    i2c.bus[device][control] =
      [&actual_value](mock::i2c::operation p_operation,
                      std::byte p_byte) -> mock::i2c::response {
      expect(p_operation == mock::i2c::operation::write);
      actual_value = p_byte;
      return { .rule = mock::i2c::increment_rule::increment };
    };

    // Exercise
    std::array<std::byte, 2> payload{ control, expected_value };
    auto result = write(i2c, device, payload);

    // Verify
    expect(expected_value == actual_value);
    expect(bool{ result });
  };

  "[failure] write"_test = []() {
    // Setup
    // Exercise
    // Verify
  };

  "[success] read"_test = []() {
    // Setup
    mock::i2c i2c;
    // Exercise
    auto result1 = write_then_read<1>(i2c, device, write_payload).value();
    // Verify
    expect(that % result1[0] == original_status);
  };

  "[failure] read"_test = []() {
    // Setup
    // Exercise
    // Verify
  };

  "[success] read<Length>"_test = []() {
    // Setup
    // Exercise
    // Verify
  };

  "[failure] read<Length>"_test = []() {
    // Setup
    // Exercise
    // Verify
  };

  "[success] write_then_read"_test = []() {
    // Setup
    // Exercise
    // Verify
  };

  "[failure] write_then_read"_test = []() {
    // Setup
    // Exercise
    // Verify
  };

  "[success] write_then_read<Length>"_test = []() {
    // Setup
    // Exercise
    // Verify
  };

  "[failure] write_then_read<Length>"_test = []() {
    // Setup
    // Exercise
    // Verify
  };
};
}  // namespace embed
