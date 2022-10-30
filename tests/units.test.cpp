#include <libhal/units.hpp>

#include <libhal/math.hpp>
#include <libhal/testing.hpp>

#include <boost/ut.hpp>

namespace hal {
boost::ut::suite units_test = []() {
  using namespace boost::ut;
  using namespace std::literals;

  "cycles_per"_test = []() {
    expect(that % 4000 == cycles_per(1.0_MHz, 4ms));
    expect(that % 1'680'000 == cycles_per(140.0_MHz, 12'000us));
    expect(that % 10 == cycles_per(10.0_Hz, 1s));
    expect(that % 720'000 == cycles_per(48.0_MHz, 15ms));
    expect(that % 192'000 == cycles_per(12.0_MHz, 16ms));
    expect(that % 960'000'000 == cycles_per(8.0_MHz, 2min));
    expect(that % 57'600'000 == cycles_per(32.0_kHz, 30min));
    // Should be 600'000'000'000 but due to floating point precision loss it
    // cannot be represented
    expect(that % 599'999'971'328 == cycles_per(1000.0_MHz, 10min));

    // Result of zero means that the time period is smaller than the
    // frequency's period length
    expect(that % 0 == cycles_per(100.0_kHz, 1us));
    expect(that % 0 == cycles_per(100.0_Hz, 1ms));
  };
  "duration wavelength"_test = []() {
    expect(that % std::numeric_limits<hal::time_duration::rep>::max() ==
           wavelength<std::femto>(0.0_Hz).count());
    expect(that % 10'000 == wavelength<std::femto>(100.0_GHz).count());
    expect(that % 1'000 == wavelength<std::milli>(1.0_Hz).count());
    expect(that % 100'000 == wavelength<std::micro>(10.0_Hz).count());
    expect(that % 1'000 == wavelength<std::nano>(1.0_MHz).count());
    expect(that % 1 == wavelength<std::nano>(1.0_GHz).count());
  };

  "float wavelength"_test = []() {
    expect(equals(0.0f, wavelength(0.0_Hz)));
    expect(equals(0.00000000001f, wavelength(100.0_GHz)));
    expect(equals(1.0f, wavelength(1.0_Hz)));
    expect(equals(0.1f, wavelength(10.0_Hz)));
    expect(equals(0.000001f, wavelength(1.0_MHz)));
    expect(equals(0.000000001f, wavelength(1.0_GHz)));
  };

  "duration_from_cycles"_test = []() {
    expect(that % 1'400us == duration_from_cycles(1.0_MHz, 1400).value());
    expect(that % 2'380'928ns == duration_from_cycles(14.0_MHz, 33333).value());
    expect(that % 10'250ms ==
           std::chrono::duration_cast<std::chrono::milliseconds>(
             duration_from_cycles(1.0_kHz, 10'250).value()));
    expect(that % 12'000us ==
           duration_from_cycles(1000.0_MHz, 12'000'000).value());
    expect(that % 0us == duration_from_cycles(1000.0_MHz, 0).value());

    expect(that % 1ns == duration_from_cycles(1000.0_MHz, 1).value());
    expect(that % 1us == duration_from_cycles(1.0_MHz, 1).value());
    expect(that % 1ms == duration_from_cycles(1.0_kHz, 1).value());
    expect(that % 1s == duration_from_cycles(1.0_Hz, 1).value());

    expect(that % 2'000'000us ==
           duration_from_cycles(500.0_kHz, 1'000'000).value());
    expect(that % 1s ==
           duration_from_cycles(1000.0_MHz, 1'000'000'000).value());
    expect(that % 200ms ==
           duration_from_cycles(1000.0_MHz, 200'000'000).value());

    expect(that % 25ms == duration_from_cycles(2000.0_MHz, 50'000'000).value());

    expect(!duration_from_cycles(
      0.01_Hz,
      static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max())));
    expect(!duration_from_cycles(
      0.01_Hz,
      static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::min())));
  };
};
}  // namespace hal
