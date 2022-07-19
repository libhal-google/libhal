/*
#include <boost/ut.hpp>
#include <libhal/spi/bit_bang_spi.hpp>
#include <span>

namespace hal {
boost::ut::suite spi_bit_bang_test = []() {
  using namespace boost::ut;

  hal::mock::counter counter;
  hal::mock::output_pin output_pin;
  hal::mock::input_pin input_pin;
  hal::mock::output_pin clock;

  "hal::mock::bit_bang_spi::constructor"_test = []() {
    // Set Up
    // Exercise
    // Verify
  };

  "hald::mock::bit_bang_spi::configure"_test = []() {
    // Setup
    constexpr hal::spi::settings expected1 = { .clock_rate = frequency(1'000),
                                               .clock_idles_high = false,
                                               .data_valid_on_trailing_edge =
                                                 false };

    constexpr hal::spi::settings expected2 = { .clock_rate = frequency(10'000),
                                               .clock_idles_high = true,
                                               .data_valid_on_trailing_edge =
                                                 true };
    hal::mock::bit_bang_spi mock(counter, output_pin, input_pin, clock);
    // Exercise
    auto result1 = mock.configure(expected1);
    auto result2 = mock.configure(expected2);
    // Verify
    expect(expected1 == std::get<0>(mock.spy_configure.call_history().at(0)));
    expect(expected2 == std::get<0>(mock.spy_configure.call_history().at(1)));
    expect(bool{ result1 });
    expect(bool{ result2 });
  };
  "hal::mock::bit_bang_spi::transfer"_test = []() {
    // Setup
    // Exercise
    // Verify
  };
};
}
*/