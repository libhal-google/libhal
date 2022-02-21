#include <boost/ut.hpp>
#include <libembeddedhal/frequency.hpp>

namespace {
template<typename Rep, typename Period>
std::ostream& operator<<(std::ostream& os,
                         const std::chrono::duration<Rep, Period>& p_duration)
{
  return (os << p_duration.count() << " * (" << Period::num << "/"
             << Period::den << ")s");
}
}  // namespace

namespace embed {
std::ostream& operator<<(std::ostream& os, const frequency& p_frequency)
{
  return (os << p_frequency.cycles_per_second() << "_Hz");
}

std::ostream& operator<<(std::ostream& os, const duty_cycle& p_duty)
{
  return (os << "duty_cycle { high: " << p_duty.high << ","
             << " low: " << p_duty.low << " }");
}

boost::ut::suite frequency_user_defined_literals_test = []() {
  using namespace boost::ut;
  using namespace std::literals;
  using namespace embed::literals;

  "frequency user defined literals"_test = []() {
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

  "frequency divider"_test = []() {
    expect(that % 333 == (1_MHz / 3_kHz));
    expect(that % 11667 == (140_MHz / 12_kHz));
    expect(that % 623 == (48_MHz / 77_kHz));
    expect(that % 133 == (133_MHz / 1_MHz));
    expect(that % 120'000 == (12_MHz / 100_Hz));
    expect(that % 160'000 == (8_MHz / 50_Hz));
    expect(that % 40 == (53_kHz / 1337_Hz));

    // Result of zero should occur if the numerator is less than the denominator
    expect(that % 0 == (10_kHz / 12_MHz));
    expect(that % 0 == (50_MHz / 122_MHz));
    expect(that % 0 == (100_Hz / 33_MHz));
    expect(that % 0 == (667_kHz / 932_MHz));
    expect(that % 0 == (1234_Hz / 54_MHz));
  };

  "frequency cycles_per"_test = []() {
    expect(that % 4 == (1_MHz).cycles_per(4us));
    expect(that % 1680 == (140_MHz).cycles_per(12'000ns));
    expect(that % 10 == (10_Hz).cycles_per(1s));
    expect(that % 720 == (48_MHz).cycles_per(15us));
    expect(that % 192 == (12_MHz).cycles_per(16us));
    expect(that % 960'000'000 == (8_MHz).cycles_per(2min));
    expect(that % 115'200'000 == (32_kHz).cycles_per(1h));

    // Result of zero means that the time period is smaller than the frequency's
    // period length
    expect(that % 0 == (1_MHz).cycles_per(100ns));
    expect(that % 0 == (100_Hz).cycles_per(1ms));
    expect(that % 0 == (100_kHz).cycles_per(2us));
  };

  "frequency duration from cycles"_test = []() {
    expect(that % 1400us == (1_MHz).duration_from_cycles(1400));
    expect(that % 2380943ns == (14_MHz).duration_from_cycles(33333));
    expect(that % 10'250ms == (1_kHz).duration_from_cycles(10'250));
    expect(that % 12'000'000ns == (1000_MHz).duration_from_cycles(12'000'000));
    expect(that % 0ns == (1000_MHz).duration_from_cycles(0));

    expect(that % 1ns == (1000_MHz).duration_from_cycles(1));
    expect(that % 1us == (1_MHz).duration_from_cycles(1));
    expect(that % 1ms == (1_kHz).duration_from_cycles(1));
    expect(that % 1s == (1_Hz).duration_from_cycles(1));

    expect(that % 1s == (1000_MHz).duration_from_cycles(1'000'000'000));
    expect(that % 1000s == (1000_MHz).duration_from_cycles(1'000'000'000'000));
    expect(that % 2500s == (4000_MHz).duration_from_cycles(10'000'000'000'000));
    expect(that % 2000s == (500_kHz).duration_from_cycles(1'000'000'000));
  };

  "frequency scalar operator"_test = []() {
    expect(eq(2_MHz, (1_MHz * 2)));
    expect(eq(45_kHz, (15_kHz * 3)));
    expect(eq(5148_kHz, (78 * 66_kHz)));
    expect(eq(32_MHz, (32 * 1_MHz)));

    expect(eq(500_kHz, (1_MHz / 2)));
    expect(eq(5_kHz, (15_kHz / 3)));
    expect(eq(2143_Hz, (15_kHz / 7)));
    expect(eq(11_kHz, (66_kHz / 6)));
    expect(eq(31250_Hz, (1_MHz / 32)));
  };

  "frequency duty_cycle"_test = []() {
    expect(eq(duty_cycle{ 280000, 0 },
              (14_MHz).calculate_duty_cycle(20ms, percent(1.00))));
    expect(eq(duty_cycle{ 277200, 2800 },
              (14_MHz).calculate_duty_cycle(20ms, percent(0.99))));
    expect(eq(duty_cycle{ 238000, 42000 },
              (14_MHz).calculate_duty_cycle(20ms, percent(0.85))));
    expect(eq(duty_cycle{ 126000, 154000 },
              (14_MHz).calculate_duty_cycle(20ms, percent(0.45))));
    expect(eq(duty_cycle{ 70'000, 210'000 },
              (14_MHz).calculate_duty_cycle(20ms, percent(0.25))));
    expect(eq(duty_cycle{ 14000, 266000 },
              (14_MHz).calculate_duty_cycle(20ms, percent(0.05))));
    expect(eq(duty_cycle{ 2800, 277200 },
              (14_MHz).calculate_duty_cycle(20ms, percent(0.01))));
    expect(eq(duty_cycle{ 0, 280000 },
              (14_MHz).calculate_duty_cycle(20ms, percent(0.00))));

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
  };
};
}  // namespace embed
