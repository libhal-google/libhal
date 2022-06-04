#include <boost/ut.hpp>
#include <libembeddedhal/frequency.hpp>

#include "ostreams.hpp"

namespace embed {
boost::ut::suite frequency_user_defined_literals_test = []() {
  using namespace boost::ut;
  using namespace std::literals;
  using namespace embed::literals;

  "frequency::user defined literals"_test = []() {
    expect(that % 1'000'000 == (1_MHz).cycles_per_second());
    expect(that % 8'000'000 == (8_MHz).cycles_per_second());
    expect(that % 48'000'000 == (48_MHz).cycles_per_second());
    expect(that % 133'000'000 == (133_MHz).cycles_per_second());
    expect(that % 140'000'000 == (140_MHz).cycles_per_second());

    expect(that % 12'000 == (12_kHz).cycles_per_second());
    expect(that % 77'000 == (77_kHz).cycles_per_second());
    expect(that % 53'000 == (53_kHz).cycles_per_second());
    expect(that % 674'000 == (674_kHz).cycles_per_second());
    expect(that % 927'000 == (927_kHz).cycles_per_second());
    expect(that % 714'000 == (714_kHz).cycles_per_second());
    expect(that % 714'834'000 == (714'834_kHz).cycles_per_second());

    expect(that % 50 == (50_Hz).cycles_per_second());
    expect(that % 1337 == (1337_Hz).cycles_per_second());
    expect(that % 100 == (100_Hz).cycles_per_second());
    expect(that % 1476 == (1476_Hz).cycles_per_second());
    expect(that % 1290 == (1290_Hz).cycles_per_second());
    expect(that % 8762 == (8762_Hz).cycles_per_second());
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
    expect(that % 250000 == (1_MHz).divide(4_Hz));
    expect(that % 11667 == (140_MHz).divide(12_kHz));
    expect(that % 10 == (10_Hz).divide(1_Hz));
    expect(that % 3200 == (48_MHz).divide(15_kHz));
    expect(that % 2000 == (12_MHz).divide(6_kHz));
    expect(that % 4 == (8_MHz).divide(2_MHz));
    expect(that % 2462 == (32_kHz).divide(13_Hz));
    expect(that % 10'000'000 == (1000_MHz).divide(100_Hz));

    // Result of zero means that the time period is smaller than the frequency's
    // period length
    expect(that % 0 == (1_MHz).divide(100_MHz));
    expect(that % 0 == (100_Hz).divide(1200_Hz));
    expect(that % 0 == (100_kHz).divide(101_kHz));
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
    expect(that % 4 == (1_MHz).cycles_per(4us).value());
    expect(that % 1680 == (140_MHz).cycles_per(12'000ns).value());
    expect(that % 10 == (10_Hz).cycles_per(1s).value());
    expect(that % 720 == (48_MHz).cycles_per(15us).value());
    expect(that % 192 == (12_MHz).cycles_per(16us).value());
    expect(that % 960'000'000 == (8_MHz).cycles_per(2min).value());
    expect(that % 115'200'000 == (32_kHz).cycles_per(1h).value());
    expect(that % 3'600'000'000'000 == (1000_MHz).cycles_per(1h).value());

    // Result of zero means that the time period is smaller than the frequency's
    // period length
    expect(that % 0 == (1_MHz).cycles_per(100ns).value());
    expect(that % 0 == (100_Hz).cycles_per(1ms).value());
    expect(that % 0 == (100_kHz).cycles_per(2us).value());
  };

  "frequency::cycles_per -> failure"_test = []() {
    expect(throws([]() { (4200_MHz).cycles_per(80'000'000min).value(); }));
    expect(throws([]() { (2500_MHz).cycles_per(160'000'000min).value(); }));
  };

  "frequency::duration_from_cycles"_test = []() {
    expect(that % 1400us == (1_MHz).duration_from_cycles(1400).value());
    expect(that % 2380929ns == (14_MHz).duration_from_cycles(33333).value());
    expect(that % 10'250ms == (1_kHz).duration_from_cycles(10'250).value());
    expect(that % 12'000'000ns ==
           (1000_MHz).duration_from_cycles(12'000'000).value());
    expect(that % 0ns == (1000_MHz).duration_from_cycles(0).value());

    expect(that % 1ns == (1000_MHz).duration_from_cycles(1).value());
    expect(that % 1us == (1_MHz).duration_from_cycles(1).value());
    expect(that % 1ms == (1_kHz).duration_from_cycles(1).value());
    expect(that % 1s == (1_Hz).duration_from_cycles(1).value());

    expect(that % 1s == (1000_MHz).duration_from_cycles(1'000'000'000).value());
    expect(that % 1000s ==
           (1000_MHz).duration_from_cycles(1'000'000'000'000).value());
    expect(that % 2500s ==
           (4000_MHz).duration_from_cycles(10'000'000'000'000).value());
    expect(that % 2000s ==
           (500_kHz).duration_from_cycles(1'000'000'000).value());
  };

  "frequency::duration_from_cycles -> failure"_test = []() {
    // Failure Condition: overflow on multiply cycles * std::nano::den
    expect(throws([]() -> auto {
      return (1_Hz).duration_from_cycles(1'000'000'000'000'000'000ULL).value();
    }));
    // Failure Condition: result cannot fit in uint64_t
    expect(throws([]() -> auto {
      return (1_Hz).duration_from_cycles(4'500'000'000'000ULL).value();
    }));
  };

  "frequency::duty_cycle"_test = []() {
    expect(eq(duty_cycle{ 280000, 0 },
              (14_MHz).calculate_duty_cycle(20ms, percent(1.00)).value()));
    expect(eq(duty_cycle{ 277200, 2800 },
              (14_MHz).calculate_duty_cycle(20ms, percent(0.99)).value()));
    expect(eq(duty_cycle{ 238000, 42000 },
              (14_MHz).calculate_duty_cycle(20ms, percent(0.85)).value()));
    expect(eq(duty_cycle{ 126000, 154000 },
              (14_MHz).calculate_duty_cycle(20ms, percent(0.45)).value()));
    expect(eq(duty_cycle{ 70'000, 210'000 },
              (14_MHz).calculate_duty_cycle(20ms, percent(0.25)).value()));
    expect(eq(duty_cycle{ 14000, 266000 },
              (14_MHz).calculate_duty_cycle(20ms, percent(0.05)).value()));
    expect(eq(duty_cycle{ 2800, 277200 },
              (14_MHz).calculate_duty_cycle(20ms, percent(0.01)).value()));
    expect(eq(duty_cycle{ 0, 280000 },
              (14_MHz).calculate_duty_cycle(20ms, percent(0.00)).value()));

    expect(eq(duty_cycle{ 2800, 0 },
              (56_MHz).calculate_duty_cycle(20_kHz, percent(1.00))));
    expect(eq(duty_cycle{ 2240, 560 },
              (56_MHz).calculate_duty_cycle(20_kHz, percent(0.80))));
    expect(eq(duty_cycle{ 1540, 1260 },
              (56_MHz).calculate_duty_cycle(20_kHz, percent(0.55))));
    expect(eq(duty_cycle{ 924, 1876 },
              (56_MHz).calculate_duty_cycle(20_kHz, percent(0.33))));
    expect(eq(duty_cycle{ 336, 2464 },
              (56_MHz).calculate_duty_cycle(20_kHz, percent(0.12))));
    expect(eq(duty_cycle{ 0, 2800 },
              (56_MHz).calculate_duty_cycle(20_kHz, percent(0.00))));

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
};
}  // namespace embed
