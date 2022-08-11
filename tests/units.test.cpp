#include <boost/ut.hpp>
#include <libhal/math.hpp>
#include <libhal/units.hpp>

#include "ostreams.hpp"

namespace hal {
boost::ut::suite units_test = []() {
  using namespace boost::ut;
  using namespace std::literals;
  using namespace hal::experimental::literals;

  "cycles_per"_test = []() {
    expect(4000 == cycles_per(1.0_MHz, 4ms));
    expect(1'680'000 == cycles_per(140.0_MHz, 12'000us));
    expect(10 == cycles_per(10.0_Hz, 1s));
    expect(720'000 == cycles_per(48.0_MHz, 15ms));
    expect(192'000 == cycles_per(12.0_MHz, 16ms));
    expect(960'000'000 == cycles_per(8.0_MHz, 2min));
    expect(57'600'000 == cycles_per(32.0_kHz, 30min));
    expect(600'000'000'000 == cycles_per(1000.0_MHz, 10min));

    // Result of zero means that the time period is smaller than the
    // frequency's period length
    expect(0 == cycles_per(100.0_kHz, 1us));
    expect(0 == cycles_per(100.0_Hz, 1ms));
  };
  "duration wavelength"_test = []() {
    expect(0 == wavelength<std::femto>(0.0_Hz).count());
    expect(10'000 == wavelength<std::femto>(100.0_GHz).count());
    expect(1'000 == wavelength<std::milli>(1.0_Hz).count());
    expect(100'000 == wavelength<std::micro>(10.0_Hz).count());
    expect(1'000 == wavelength<std::nano>(1.0_MHz).count());
    expect(1 == wavelength<std::nano>(1.0_GHz).count());
  };

  "float wavelength"_test = []() {
    expect(equals(0.0f, wavelength<float>(0.0_Hz)));
    expect(equals(0.00000000001f, wavelength<float>(100.0_GHz)));
    expect(equals(1.0f, wavelength<float>(1.0_Hz)));
    expect(equals(0.1f, wavelength<float>(10.0_Hz)));
    expect(equals(0.000001f, wavelength<float>(1.0_MHz)));
    expect(equals(0.000000001f, wavelength<float>(1.0_GHz)));

    expect(equals(0.0, wavelength<double>(0.0_Hz)));
    expect(equals(0.00000000001, wavelength<double>(100.0_GHz)));
    expect(equals(1.0, wavelength<double>(1.0_Hz)));
    expect(equals(0.1, wavelength<double>(10.0_Hz)));
    expect(equals(0.000001, wavelength<double>(1.0_MHz)));
    expect(equals(0.000000001, wavelength<double>(1.0_GHz)));

    expect(equals(0.0, wavelength<>(0.0_Hz)));
    expect(equals(0.00000000001, wavelength<>(100.0_GHz)));
    expect(equals(1.0, wavelength<>(1.0_Hz)));
    expect(equals(0.1, wavelength<>(10.0_Hz)));
    expect(equals(0.000001, wavelength<>(1.0_MHz)));
    expect(equals(0.000000001, wavelength<>(1.0_GHz)));
  };

  "duration_from_cycles"_test = []() {
    expect(that % 1'400us == duration_from_cycles(1.0_MHz, 1400).value());
    expect(that % 2'380'928ns == duration_from_cycles(14.0_MHz, 33333).value());
    expect(that % 10'250ms == duration_from_cycles(1.0_kHz, 10'250).value());
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

    expect(
      !duration_from_cycles(0.01_Hz, std::numeric_limits<std::int32_t>::max()));
    expect(
      !duration_from_cycles(0.01_Hz, std::numeric_limits<std::int32_t>::min()));
  };
};
}  // namespace hal
