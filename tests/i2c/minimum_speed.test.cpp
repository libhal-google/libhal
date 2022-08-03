#include <boost/ut.hpp>

#include <libhal/i2c/minimum_speed.hpp>
#include <libhal/testing.hpp>

namespace hal {

struct fake_i2c : public hal::i2c
{
  void reset()
  {
    spy_configure.reset();
    spy_transaction.reset();
  }
  // Spy handler for hal::i2c::configure()
  spy_handler<settings> spy_configure;
  /// Record of the out data from hal::i2c::transaction()
  spy_handler<std::byte,
              std::span<const std::byte>,
              std::span<std::byte>,
              std::function<hal::timeout>>
    spy_transaction;

private:
  status driver_configure(const settings& p_settings) noexcept
  {
    return spy_configure.record(p_settings);
  }
  status driver_transaction(
    [[maybe_unused]] std::byte p_address,
    [[maybe_unused]] std::span<const std::byte> p_data_out,
    [[maybe_unused]] std::span<std::byte> p_data_in,
    [[maybe_unused]] std::function<hal::timeout> p_timeout) noexcept
  {
    return spy_transaction.record(p_address, p_data_out, p_data_in, p_timeout);
  }
};
}  // namespace hal

namespace hal {
boost::ut::suite minimum_speed_test = []() {
  using namespace boost::ut;

  "hal::i2c::minimum_speed_i2c::create() with default frequency + configure()"_test =
    []() {
      // Setup
      hal::fake_i2c mock_i2c;
      constexpr hal::i2c::settings minimum_default = {
        .clock_rate = minimum_speed_i2c::default_max_speed
      };
      constexpr hal::i2c::settings expected_upper_boundary = {
        .clock_rate = frequency(3'000'000)
      };
      constexpr hal::i2c::settings expected_lower = { .clock_rate =
                                                        frequency(1) };
      constexpr hal::i2c::settings expected_zero = { .clock_rate =
                                                       frequency(0) };

      // Exercise
      auto mock = hal::minimum_speed_i2c::create(mock_i2c);

      auto result1 = mock.configure(expected_upper_boundary);
      auto result2 = mock.configure(minimum_default);
      auto result3 = mock.configure(expected_lower);
      auto result4 = mock.configure(expected_zero);

      // Verify
      expect(bool{ result1 });
      expect(bool{ result2 });
      expect(bool{ result3 });
      expect(!result4);
      expect(expected_lower ==
             std::get<0>(mock_i2c.spy_configure.call_history().at(0)));
    };

  "hal::i2c::minimum_speed_i2c::create() + configure() with frequency"_test =
    []() {
      // Setup
      hal::fake_i2c mock_i2c;
      constexpr hal::frequency device_frequency = frequency(1'000'000);
      constexpr hal::i2c::settings choosen_frequency = { .clock_rate =
                                                           device_frequency };
      constexpr hal::i2c::settings expected_upper_boundary = {
        .clock_rate = frequency(3'000'000)
      };
      constexpr hal::i2c::settings expected_lower = { .clock_rate =
                                                        frequency(1) };
      constexpr hal::i2c::settings expected_zero = { .clock_rate =
                                                       frequency(0) };

      // Exercise
      auto mock = hal::minimum_speed_i2c::create(mock_i2c, device_frequency);

      auto result1 = mock.configure(expected_upper_boundary);
      auto result2 = mock.configure(choosen_frequency);
      auto result3 = mock.configure(expected_lower);
      auto result4 = mock.configure(expected_zero);

      // Verify
      expect(bool{ result1 });
      expect(bool{ result2 });
      expect(bool{ result3 });
      expect(!result4);
      expect(expected_lower ==
             std::get<0>(mock_i2c.spy_configure.call_history().at(0)));
    };

  "hal::i2c::minimum_speed_i2c::transcation"_test = []() {
    // Setup
    constexpr std::byte expected_address{ 0xAA };
    constexpr std::array<const std::byte, 2> data_out{ std::byte{ 0xAB },
                                                       std::byte{ 0xFF } };
    std::span<std::byte> data_in;
    bool has_been_called = false;
    std::function<hal::timeout> expected_timeout =
      [&has_been_called]() -> status {
      has_been_called = true;
      return {};
    };
    hal::fake_i2c mock_i2c;
    auto mock = hal::minimum_speed_i2c::create(mock_i2c);

    // Exercise
    auto result1 =
      mock.transaction(expected_address, data_out, data_in, expected_timeout);

    // Verify
    auto transaction_call_info = mock_i2c.spy_transaction.call_history().at(0);
    auto transaction_expected_address = std::get<0>(transaction_call_info);
    auto transaction_data_out = std::get<1>(transaction_call_info);
    auto transaction_data_in = std::get<2>(transaction_call_info);
    auto transaction_expected_timeout = std::get<3>(transaction_call_info);

    expect(expected_address == transaction_expected_address);
    expect(data_out.data() == transaction_data_out.data());
    expect(data_out.size() == transaction_data_out.size());
    expect(data_in.data() == transaction_data_in.data());
    expect(data_in.size() == transaction_data_in.size());
    transaction_expected_timeout();
    expect(has_been_called);
  };
};
}  // namespace hal
