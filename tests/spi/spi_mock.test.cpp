#include <algorithm>
#include <boost/ut.hpp>
#include <libembeddedhal/spi/spi_mock.hpp>

namespace embed {
boost::ut::suite spi_mock_test = []() {
  using namespace boost::ut;

  "embed::mock::spi::configure()"_test = []() {
    // Setup
    constexpr embed::spi::settings expected1 = {
        .clock_rate = frequency(1'000),
        .clock_idles_high = false,
        .data_valid_on_trailing_edge = false};

    constexpr embed::spi::settings expected2 = {
        .clock_rate = frequency(10'000),
        .clock_idles_high = true,
        .data_valid_on_trailing_edge = true};

    embed::mock::spi mock;
    mock.spy_configure.trigger_error_on_call(3);

    // Exercise + Verify
    expect(bool{mock.configure(expected1)});
    expect(expected1 == std::get<0>(mock.spy_configure.call_history().at(0)));

    expect(bool{mock.configure(expected2)});
    expect(expected2 == std::get<0>(mock.spy_configure.call_history().at(1)));

    expect(!mock.configure(expected2));
    expect(expected2 == std::get<0>(mock.spy_configure.call_history().at(2)));
  };

  "embed::mock::spi::transfer()"_test = []() {
    // Setup
    constexpr std::array<const std::byte, 1> data_out_expected1{std::byte{0xAA}};
    std::array<std::byte, 1> data_in_expected1{std::byte{0xBB}};
    constexpr std::byte filler_expected1{0xCC};

    constexpr std::array<const std::byte, 1> data_out_expected2{std::byte{0xDD}};
    std::array<std::byte, 1> data_in_expected2{std::byte{0xEE}};
    constexpr std::byte filler_expected2{0xFF};

    constexpr auto compare_span = [](auto lhs, auto rhs) -> bool {
      return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    };

    embed::mock::spi mock;
    mock.spy_transfer.trigger_error_on_call(3);

    // Exercise + Verify
    expect(bool{mock.transfer(data_out_expected1, data_in_expected1,
                              filler_expected1)});
    expect(compare_span(data_out_expected1, std::get<0>(mock.spy_transfer.call_history().at(0))));
    expect(compare_span(data_in_expected1, std::get<1>(mock.spy_transfer.call_history().at(0))));
    expect(filler_expected1 == std::get<2>(mock.spy_transfer.call_history().at(0)));

    expect(bool{mock.transfer(data_out_expected2, data_in_expected2,
                              filler_expected2)});
    expect(compare_span(data_out_expected2, std::get<0>(mock.spy_transfer.call_history().at(1))));
    expect(compare_span(data_in_expected2, std::get<1>(mock.spy_transfer.call_history().at(1))));
    expect(filler_expected2 == std::get<2>(mock.spy_transfer.call_history().at(1)));

    expect(!mock.transfer(data_out_expected2, data_in_expected2, filler_expected2));
    expect(compare_span(data_out_expected2, std::get<0>(mock.spy_transfer.call_history().at(2))));
    expect(compare_span(data_in_expected2, std::get<1>(mock.spy_transfer.call_history().at(2))));
    expect(filler_expected2 == std::get<2>(mock.spy_transfer.call_history().at(2)));
  };
};
}  // namespace embed
