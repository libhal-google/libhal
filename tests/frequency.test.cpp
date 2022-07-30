#include <array>
#include <boost/ut.hpp>
#include <libembeddedhal/frequency.hpp>

#include "ostreams.hpp"

namespace hal {
boost::ut::suite frequency_user_defined_literals_test = []() {
  using namespace boost::ut;
  using namespace std::literals;
  using namespace hal::literals;

  "frequency::user defined literals"_test = []() {
    expect(that % 1'000'000 == (1_MHz).value_hz);
    expect(that % 8'000'000 == (8_MHz).value_hz);
    expect(that % 48'000'000 == (48_MHz).value_hz);
    expect(that % 133'000'000 == (133_MHz).value_hz);
    expect(that % 140'000'000 == (140_MHz).value_hz);

    expect(that % 12'000 == (12_kHz).value_hz);
    expect(that % 77'000 == (77_kHz).value_hz);
    expect(that % 53'000 == (53_kHz).value_hz);
    expect(that % 674'000 == (674_kHz).value_hz);
    expect(that % 927'000 == (927_kHz).value_hz);
    expect(that % 714'000 == (714_kHz).value_hz);
    expect(that % 714'834'000 == (714'834_kHz).value_hz);

    expect(that % 50 == (50_Hz).value_hz);
    expect(that % 1337 == (1337_Hz).value_hz);
    expect(that % 100 == (100_Hz).value_hz);
    expect(that % 1476 == (1476_Hz).value_hz);
    expect(that % 1290 == (1290_Hz).value_hz);
    expect(that % 8762 == (8762_Hz).value_hz);
  };

  "frequency::operator<=>"_test = []() {
    // ==
    expect(that % frequency{ 1'000'000 } == 1_MHz);
    expect(that % frequency{ 8'000'000 } == 8_MHz);
    expect(that % frequency{ 48'000'000 } == 48_MHz);
    expect(that % frequency{ 133'000'000 } == 133_MHz);
    expect(that % frequency{ 140'000'000 } == 140_MHz);

    // >
    expect(that % frequency{ 14'000 } > 12_kHz);
    expect(that % frequency{ 80'000 } > 77_kHz);
    expect(that % frequency{ 55'000 } > 53_kHz);
    expect(that % frequency{ 700'000 } > 674_kHz);
    expect(that % frequency{ 1'000'000 } > 927_kHz);
    expect(that % frequency{ 715'000 } > 714_kHz);
    expect(that % frequency{ 714'834'001 } > 714834_kHz);

    // <
    expect(that % frequency{ 40 } < 50_Hz);
    expect(that % frequency{ 1336 } < 1337_Hz);
    expect(that % frequency{ 99 } < 100_Hz);
    expect(that % frequency{ 1475 } < 1476_Hz);
    expect(that % frequency{ 5 } < 1290_Hz);
    expect(that % frequency{ 4444 } < 8762_Hz);
  };

  "frequency::divide"_test = []() {
    expect(that % 250000 == (1_MHz / 4_Hz));
    expect(that % 11667 == (140_MHz / 12_kHz));
    expect(that % 10 == (10_Hz / 1_Hz));
    expect(that % 3200 == (48_MHz / 15_kHz));
    expect(that % 2000 == (12_MHz / 6_kHz));
    expect(that % 4 == (8_MHz / 2_MHz));
    expect(that % 2462 == (32_kHz / 13_Hz));
    expect(that % 10'000'000 == (1000_MHz / 100_Hz));

    // Result of zero means that the time period is smaller than the frequency's
    // period length
    expect(that % 0 == (1_MHz / 100_MHz));
    expect(that % 0 == (100_Hz / 1200_Hz));
    expect(that % 0 == (100_kHz / 101_kHz));
  };

  "frequency::operator/(frequency)"_test = []() {
    expect(that % 250000 == (1_MHz / 4_Hz));
    expect(that % 11667 == (140_MHz / 12_kHz));
    expect(that % 10 == (10_Hz / 1_Hz));
    expect(that % 3200 == (48_MHz / 15_kHz));
    expect(that % 2000 == (12_MHz / 6_kHz));
    expect(that % 4 == (8_MHz / 2_MHz));
    expect(that % 2462 == (32_kHz / 13_Hz));
    expect(that % 10'000'000 == (1000_MHz / 100_Hz));

    // Result of zero means that the time period is smaller than the frequency's
    // period length
    expect(that % 0 == (1_MHz / 100_MHz));
    expect(that % 0 == (100_Hz / 1200_Hz));
    expect(that % 0 == (100_kHz / 201_kHz));
  };

  "frequency::operator/(int)"_test = []() {
    expect(that % 250000_Hz == (1_MHz / 4));
    expect(that % 11667_Hz == (140_MHz / 12'000));
    expect(that % 10_Hz == (10_Hz / 1));
    expect(that % 3200_Hz == (48_MHz / 15'000));
    expect(that % 2000_Hz == (12_MHz / 6'000));
    expect(that % 4_Hz == (8_MHz / 2'000'000));
    expect(that % 2462_Hz == (32_kHz / 13));
    expect(that % 10'000'000_Hz == (1000_MHz / 100));

    // Result of zero means that the time period is smaller than the frequency's
    // period length
    expect(that % 0_Hz == (1_MHz / 100'000'000));
    expect(that % 0_Hz == (100_Hz / 1200));
    expect(that % 0_Hz == (100_kHz / 201'000));
  };

  "frequency::cycles_per"_test = []() {
    expect(that % 4000 == cycles_per(1_MHz, 4ms));
    expect(that % 1'680'000 == cycles_per(140_MHz, 12'000us));
    expect(that % 10 == cycles_per(10_Hz, 1s));
    expect(that % 720'000 == cycles_per(48_MHz, 15ms));
    expect(that % 192'000 == cycles_per(12_MHz, 16ms));
    expect(that % 960'000'000 == cycles_per(8_MHz, 2min));
    expect(that % 57'600'000 == cycles_per(32_kHz, 30min));
    expect(that % 600'000'000'000 == cycles_per(1000_MHz, 10min));

    // Result of zero means that the time period is smaller than the frequency's
    // period length
    expect(that % 0 == cycles_per(100_kHz, 1us));
    expect(that % 0 == cycles_per(100_Hz, 1ms));
  };

  "frequency::duration_from_cycles"_test = []() {
    expect(that % 1'400us == duration_from_cycles(1_MHz, 1400));
    expect(that % 2380929ns == duration_from_cycles(14_MHz, 33333));
    expect(that % 10'250ms == duration_from_cycles(1_kHz, 10'250));
    expect(that % 12'000us == duration_from_cycles(1000_MHz, 12'000'000));
    expect(that % 0us == duration_from_cycles(1000_MHz, 0));

    expect(that % 1ns == duration_from_cycles(1000_MHz, 1));
    expect(that % 1us == duration_from_cycles(1_MHz, 1));
    expect(that % 1ms == duration_from_cycles(1_kHz, 1));
    expect(that % 1s == duration_from_cycles(1_Hz, 1));

    expect(that % 2'000'000us == duration_from_cycles(500_kHz, 1'000'000));
    expect(that % 1s == duration_from_cycles(1000_MHz, 1'000'000'000));
    expect(that % 200ms == duration_from_cycles(1000_MHz, 200'000'000));
    expect(that % 25ms == duration_from_cycles(2000_MHz, 50'000'000));
  };

  "frequency::duty_cycle"_test = []() {
    expect(eq(duty_cycle{ 280000, 0 },
              calculate_duty_cycle(14_MHz, 20ms, percent(1.00)).value()));
    expect(eq(duty_cycle{ 277200, 2800 },
              calculate_duty_cycle(14_MHz, 20ms, percent(0.99)).value()));
    expect(eq(duty_cycle{ 238000, 42000 },
              calculate_duty_cycle(14_MHz, 20ms, percent(0.85)).value()));
    expect(eq(duty_cycle{ 126000, 154000 },
              calculate_duty_cycle(14_MHz, 20ms, percent(0.45)).value()));
    expect(eq(duty_cycle{ 70'000, 210'000 },
              calculate_duty_cycle(14_MHz, 20ms, percent(0.25)).value()));
    expect(eq(duty_cycle{ 14000, 266000 },
              calculate_duty_cycle(14_MHz, 20ms, percent(0.05)).value()));
    expect(eq(duty_cycle{ 2800, 277200 },
              calculate_duty_cycle(14_MHz, 20ms, percent(0.01)).value()));
    expect(eq(duty_cycle{ 0, 280000 },
              calculate_duty_cycle(14_MHz, 20ms, percent(0.00)).value()));

    // expect(eq(duty_cycle{ 2800, 0 },
    //           calculate_duty_cycle(56_MHz, 20_kHz,
    //           percent(1.00)).value_hz));
    // expect(eq(duty_cycle{ 2240, 560 },
    //           calculate_duty_cycle(56_MHz, 20_kHz,
    //           percent(0.80)).value_hz));
    // expect(eq(duty_cycle{ 1540, 1260 },
    //           calculate_duty_cycle(56_MHz, 20_kHz,
    //           percent(0.55)).value_hz));
    // expect(eq(duty_cycle{ 924, 1876 },
    //           calculate_duty_cycle(56_MHz, 20_kHz,
    //           percent(0.33)).value_hz));
    // expect(eq(duty_cycle{ 336, 2464 },
    //           calculate_duty_cycle(56_MHz, 20_kHz,
    //           percent(0.12)).value_hz));
    // expect(eq(duty_cycle{ 0, 2800 },
    //           calculate_duty_cycle(56_MHz, 20_kHz,
    //           percent(0.00)).value_hz));

    expect(eq(static_cast<percent>(duty_cycle{ 2800, 0 }), percent(1.00)));
    expect(eq(static_cast<percent>(duty_cycle{ 2240, 560 }), percent(0.80)));
    expect(eq(static_cast<percent>(duty_cycle{ 1540, 1260 }), percent(0.55)));
    expect(eq(static_cast<percent>(duty_cycle{ 924, 1876 }), percent(0.33)));
    expect(eq(static_cast<percent>(duty_cycle{ 336, 2464 }), percent(0.12)));
    expect(eq(static_cast<percent>(duty_cycle{ 0, 2800 }), percent(0.00)));
    expect(eq(static_cast<percent>(
                duty_cycle{ std::numeric_limits<std::int32_t>::max(),
                            std::numeric_limits<std::int32_t>::max() }),
              percent(0.50)));
  };

  "frequency::closest"_test = []() {
    auto dividers = std::array<uint32_t, 6>{ 1, 2, 4, 8, 16, 32 };
    auto dividers_no_order = std::array<uint32_t, 6>{ 16, 2, 4, 10, 1, 32 };
    auto no_dividers = std::array<uint32_t, 0>{};

    // Test for empty span
    expect(eq(closest(14_MHz,
                      no_dividers.begin(),
                      no_dividers.end(),
                      1_MHz,
                      divider_rule::lower),
              no_dividers.end()));
    expect(eq((closest(14_MHz,
                       no_dividers.begin(),
                       no_dividers.end(),
                       1_MHz,
                       divider_rule::higher)),
              no_dividers.end()));
    expect(eq((closest(14_MHz,
                       no_dividers.begin(),
                       no_dividers.end(),
                       1_MHz,
                       divider_rule::closest)),
              no_dividers.end()));

    // Test for no suitable candidates
    expect(eq(
      closest(
        64_MHz, dividers.begin(), dividers.end(), 1_MHz, divider_rule::lower),
      dividers.end()));
    expect(eq(
      closest(
        64_MHz, dividers.begin(), dividers.end(), 65_MHz, divider_rule::higher),
      dividers.end()));

    // Test common cases
    expect(eq(
      *closest(
        64_MHz, dividers.begin(), dividers.end(), 33_MHz, divider_rule::lower),
      2));
    expect(eq(
      *closest(
        64_MHz, dividers.begin(), dividers.end(), 33_MHz, divider_rule::higher),
      1));
    expect(eq(*closest(64_MHz,
                       dividers.begin(),
                       dividers.end(),
                       33_MHz,
                       divider_rule::closest),
              2));

    expect(eq(
      *closest(
        64_MHz, dividers.begin(), dividers.end(), 7_MHz, divider_rule::lower),
      16));
    expect(eq(
      *closest(
        64_MHz, dividers.begin(), dividers.end(), 7_MHz, divider_rule::higher),
      8));
    expect(eq(
      *closest(
        64_MHz, dividers.begin(), dividers.end(), 7_MHz, divider_rule::closest),
      8));

    expect(eq(
      closest(
        64_MHz, dividers.begin(), dividers.end(), 1_MHz, divider_rule::lower),
      dividers.end()));
    expect(eq(
      *closest(
        64_MHz, dividers.begin(), dividers.end(), 1_MHz, divider_rule::higher),
      32));
    expect(eq(
      *closest(
        64_MHz, dividers.begin(), dividers.end(), 1_MHz, divider_rule::closest),
      32));

    // Test unordered span
    expect(eq(*closest(64_MHz,
                       dividers_no_order.begin(),
                       dividers_no_order.end(),
                       33_MHz,
                       divider_rule::lower),
              2));
    expect(eq(*closest(64_MHz,
                       dividers_no_order.begin(),
                       dividers_no_order.end(),
                       33_MHz,
                       divider_rule::higher),
              1));
    expect(eq(*closest(64_MHz,
                       dividers_no_order.begin(),
                       dividers_no_order.end(),
                       33_MHz,
                       divider_rule::closest),
              2));

    expect(eq(*closest(64_MHz,
                       dividers_no_order.begin(),
                       dividers_no_order.end(),
                       7_MHz,
                       divider_rule::lower),
              10));
    expect(eq(*closest(64_MHz,
                       dividers_no_order.begin(),
                       dividers_no_order.end(),
                       7_MHz,
                       divider_rule::higher),
              4));
    expect(eq(*closest(64_MHz,
                       dividers_no_order.begin(),
                       dividers_no_order.end(),
                       7_MHz,
                       divider_rule::closest),
              10));

    expect(eq(closest(64_MHz,
                      dividers_no_order.begin(),
                      dividers_no_order.end(),
                      1_MHz,
                      divider_rule::lower),
              dividers_no_order.end()));
    expect(eq(*closest(64_MHz,
                       dividers_no_order.begin(),
                       dividers_no_order.end(),
                       1_MHz,
                       divider_rule::higher),
              32));
    expect(eq(*closest(64_MHz,
                       dividers_no_order.begin(),
                       dividers_no_order.end(),
                       1_MHz,
                       divider_rule::closest),
              32));

    // Test for exact value
    expect(eq(
      *closest(
        64_MHz, dividers.begin(), dividers.end(), 2_MHz, divider_rule::lower),
      32));
    expect(eq(
      *closest(
        64_MHz, dividers.begin(), dividers.end(), 2_MHz, divider_rule::higher),
      32));
    expect(eq(
      *closest(
        64_MHz, dividers.begin(), dividers.end(), 2_MHz, divider_rule::closest),
      32));
    expect(eq(
      *closest(
        64_MHz, dividers.begin(), dividers.end(), 32_MHz, divider_rule::lower),
      2));
    expect(eq(
      *closest(
        64_MHz, dividers.begin(), dividers.end(), 32_MHz, divider_rule::higher),
      2));
    expect(eq(*closest(64_MHz,
                       dividers.begin(),
                       dividers.end(),
                       32_MHz,
                       divider_rule::closest),
              2));
  };
};
}  // namespace hal
