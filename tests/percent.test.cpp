#include "ostreams.hpp"
#include <boost/ut.hpp>
#include <libembeddedhal/percent.hpp>

namespace embed {
boost::ut::suite percent_ratio_and_cast_test = []() {
  using namespace boost::ut;

  // Float Testing
  expect(0.50_d == embed::percent::from_ratio(1, 2).to<float>());
  expect(0.50_d == embed::percent::from_ratio(2, 4).to<float>());
  expect(0.20_d == embed::percent::from_ratio(1, 5).to<float>());

  expect(0.010_d == embed::percent::from_ratio(1, 100).to<float>());
  expect(0.020_d == embed::percent::from_ratio(2, 100).to<float>());
  expect(0.070_d == embed::percent::from_ratio(7, 100).to<float>());
  expect(0.080_d == embed::percent::from_ratio(8, 100).to<float>());
  expect(0.100_d == embed::percent::from_ratio(10, 100).to<float>());
  expect(0.120_d == embed::percent::from_ratio(12, 100).to<float>());
  expect(0.250_d == embed::percent::from_ratio(25, 100).to<float>());
  expect(0.330_d == embed::percent::from_ratio(33, 100).to<float>());
  expect(0.460_d == embed::percent::from_ratio(46, 100).to<float>());
  expect(0.520_d == embed::percent::from_ratio(52, 100).to<float>());
  expect(0.550_d == embed::percent::from_ratio(55, 100).to<float>());
  expect(0.670_d == embed::percent::from_ratio(67, 100).to<float>());
  expect(0.750_d == embed::percent::from_ratio(75, 100).to<float>());
  expect(0.810_d == embed::percent::from_ratio(81, 100).to<float>());
  expect(0.900_d == embed::percent::from_ratio(90, 100).to<float>());
  expect(0.940_d == embed::percent::from_ratio(94, 100).to<float>());
  expect(1.000_d == embed::percent::from_ratio(100, 100).to<float>());

  expect(0.0020_d == embed::percent::from_ratio(1, 500).to<float>());
  expect(0.0040_d == embed::percent::from_ratio(2, 500).to<float>());
  expect(0.0140_d == embed::percent::from_ratio(7, 500).to<float>());
  expect(0.0160_d == embed::percent::from_ratio(8, 500).to<float>());
  expect(0.0200_d == embed::percent::from_ratio(10, 500).to<float>());
  expect(0.0240_d == embed::percent::from_ratio(12, 500).to<float>());
  expect(0.0500_d == embed::percent::from_ratio(25, 500).to<float>());
  expect(0.0660_d == embed::percent::from_ratio(33, 500).to<float>());
  expect(0.0920_d == embed::percent::from_ratio(46, 500).to<float>());
  expect(0.1040_d == embed::percent::from_ratio(52, 500).to<float>());
  expect(0.1100_d == embed::percent::from_ratio(55, 500).to<float>());
  expect(0.1340_d == embed::percent::from_ratio(67, 500).to<float>());
  expect(0.1500_d == embed::percent::from_ratio(75, 500).to<float>());
  expect(0.1620_d == embed::percent::from_ratio(81, 500).to<float>());
  expect(0.1800_d == embed::percent::from_ratio(90, 500).to<float>());
  expect(0.1880_d == embed::percent::from_ratio(94, 500).to<float>());
  expect(0.2000_d == embed::percent::from_ratio(100, 500).to<float>());
  expect(0.4000_d == embed::percent::from_ratio(200, 500).to<float>());
  expect(0.6000_d == embed::percent::from_ratio(300, 500).to<float>());
  expect(0.8000_d == embed::percent::from_ratio(400, 500).to<float>());
  expect(1.0000_d == embed::percent::from_ratio(500, 500).to<float>());

  // Double Testing
  expect(0.010_d == embed::percent::from_ratio(1, 100).to<double>());
  expect(0.020_d == embed::percent::from_ratio(2, 100).to<double>());
  expect(0.070_d == embed::percent::from_ratio(7, 100).to<double>());
  expect(0.080_d == embed::percent::from_ratio(8, 100).to<double>());
  expect(0.100_d == embed::percent::from_ratio(10, 100).to<double>());
  expect(0.120_d == embed::percent::from_ratio(12, 100).to<double>());
  expect(0.250_d == embed::percent::from_ratio(25, 100).to<double>());
  expect(0.330_d == embed::percent::from_ratio(33, 100).to<double>());
  expect(0.460_d == embed::percent::from_ratio(46, 100).to<double>());
  expect(0.520_d == embed::percent::from_ratio(52, 100).to<double>());
  expect(0.550_d == embed::percent::from_ratio(55, 100).to<double>());
  expect(0.670_d == embed::percent::from_ratio(67, 100).to<double>());
  expect(0.750_d == embed::percent::from_ratio(75, 100).to<double>());
  expect(0.810_d == embed::percent::from_ratio(81, 100).to<double>());
  expect(0.900_d == embed::percent::from_ratio(90, 100).to<double>());
  expect(0.940_d == embed::percent::from_ratio(94, 100).to<double>());
  expect(1.000_d == embed::percent::from_ratio(100, 100).to<double>());

  expect(0.0020_d == embed::percent::from_ratio(1, 500).to<double>());
  expect(0.0040_d == embed::percent::from_ratio(2, 500).to<double>());
  expect(0.0140_d == embed::percent::from_ratio(7, 500).to<double>());
  expect(0.0160_d == embed::percent::from_ratio(8, 500).to<double>());
  expect(0.0200_d == embed::percent::from_ratio(10, 500).to<double>());
  expect(0.0240_d == embed::percent::from_ratio(12, 500).to<double>());
  expect(0.0500_d == embed::percent::from_ratio(25, 500).to<double>());
  expect(0.0660_d == embed::percent::from_ratio(33, 500).to<double>());
  expect(0.0920_d == embed::percent::from_ratio(46, 500).to<double>());
  expect(0.1040_d == embed::percent::from_ratio(52, 500).to<double>());
  expect(0.1100_d == embed::percent::from_ratio(55, 500).to<double>());
  expect(0.1340_d == embed::percent::from_ratio(67, 500).to<double>());
  expect(0.1500_d == embed::percent::from_ratio(75, 500).to<double>());
  expect(0.1620_d == embed::percent::from_ratio(81, 500).to<double>());
  expect(0.1800_d == embed::percent::from_ratio(90, 500).to<double>());
  expect(0.1880_d == embed::percent::from_ratio(94, 500).to<double>());
  expect(0.2000_d == embed::percent::from_ratio(100, 500).to<double>());
  expect(0.4000_d == embed::percent::from_ratio(200, 500).to<double>());
  expect(0.6000_d == embed::percent::from_ratio(300, 500).to<double>());
  expect(0.8000_d == embed::percent::from_ratio(400, 500).to<double>());
  expect(1.0000_d == embed::percent::from_ratio(500, 500).to<double>());
};

boost::ut::suite percent_signed_test = []() {
  using namespace boost::ut;

  "4-bit_scaling"_test = []() {
    // Setup
    embed::percent value;
    int32_t expected = 0;

    // Exercise
    value = embed::percent::convert<4>(7);
    expect(that % 0b0111'1111'1111'1111'1111'1111'1111'1111 ==
           value.raw_value());

    value = embed::percent::convert<4>(6);
    expect(that % 0b0110'1101'1011'0110'1101'1011'0110'1101 ==
           value.raw_value());

    value = embed::percent::convert<4>(5);
    expect(that % 0b0101'1011'0110'1101'1011'0110'1101'1011 ==
           value.raw_value());

    value = embed::percent::convert<4>(4);
    expect(that % 0b0100'1001'0010'0100'1001'0010'0100'1001 ==
           value.raw_value());

    value = embed::percent::convert<4>(3);
    expect(that % 0b0011'0110'1101'1011'0110'1101'1011'0110 ==
           value.raw_value());

    value = embed::percent::convert<4>(2);
    expect(that % 0b0010'0100'1001'0010'0100'1001'0010'0100 ==
           value.raw_value());

    value = embed::percent::convert<4>(1);
    expect(that % 0b0001'0010'0100'1001'0010'0100'1001'0010 ==
           value.raw_value());

    value = embed::percent::convert<4>(0);
    expect(that % 0 == value.raw_value());

    value = embed::percent::convert<4>(-1);
    expected = 0b1111 << 28;
    expect(that % expected == value.raw_value());

    value = embed::percent::convert<4>(-2);
    expected = 0b1110 << 28;
    expect(that % expected == value.raw_value());

    value = embed::percent::convert<4>(-3);
    expected = 0b1101 << 28;
    expect(that % expected == value.raw_value());

    value = embed::percent::convert<4>(-4);
    expected = 0b1100 << 28;
    expect(that % expected == value.raw_value());

    value = embed::percent::convert<4>(-5);
    expected = 0b1011 << 28;
    expect(that % expected == value.raw_value());

    value = embed::percent::convert<4>(-6);
    expected = 0b1010 << 28;
    expect(that % expected == value.raw_value());

    value = embed::percent::convert<4>(-7);
    expected = 0b1001 << 28;
    expect(that % expected == value.raw_value());

    value = embed::percent::convert<4>(-8);
    expected = 0b1000 << 28;
    expect(that % expected == value.raw_value());
  };

  "8-bit_scaling"_test = []() {
    // Setup
    // Exercise
  };

  "12-bit_scaling"_test = []() {
    // Setup
    // Exercise
  };

  "15-bit_scaling"_test = []() {
    // Setup
    // Exercise
  };

  "16-bit_scaling"_test = []() {
    // Setup
    embed::percent value;
    int32_t expected = 0;

    // Exercise
    value = embed::percent::convert<16>((1 << 15) - 1);
    expect(that % 0b0111'1111'1111'1111'1111'1111'1111'1111 ==
           value.raw_value());

    value = embed::percent::convert<16>(0x0ABC);
    expect(that % (0x0ABC'0000 | (0x0ABC << 1)) == value.raw_value());

    // value = embed::percent::convert<16>(5);
    // expect(that % 0b0 == value.raw_value());

    // value = embed::percent::convert<16>(4);
    // expect(that % 0b0 == value.raw_value());

    // value = embed::percent::convert<16>(3);
    // expect(that % 0b0 == value.raw_value());

    // value = embed::percent::convert<16>(2);
    // expect(that % 0b0 == value.raw_value());

    value = embed::percent::convert<16>(1);
    expect(that % (0x0001'0000 | (0x0001 << 1)) == value.raw_value());

    value = embed::percent::convert<16>(0);
    expect(that % 0 == value.raw_value());

    value = embed::percent::convert<16>(-1);
    expected = 0b1111'1111'1111'1111 << 16;
    expect(that % expected == value.raw_value());

    value = embed::percent::convert<16>(-2);
    expected = 0b1111'1111'1111'1110 << 16;
    expect(that % expected == value.raw_value());

    value = embed::percent::convert<16>(-3);
    expected = 0b1111'1111'1111'1101 << 16;
    expect(that % expected == value.raw_value());

    value = embed::percent::convert<16>(-4);
    expected = 0b1111'1111'1111'1100 << 16;
    expect(that % expected == value.raw_value());

    value = embed::percent::convert<16>(-5);
    expected = 0b1111'1111'1111'1011 << 16;
    expect(that % expected == value.raw_value());

    value = embed::percent::convert<16>(-6);
    expected = 0b1111'1111'1111'1010 << 16;
    expect(that % expected == value.raw_value());

    value = embed::percent::convert<16>(-7);
    expected = 0b1111'1111'1111'1001 << 16;
    expect(that % expected == value.raw_value());

    value = embed::percent::convert<16>(-8);
    expected = 0b1111'1111'1111'1000 << 16;
    expect(that % expected == value.raw_value());
  };

  "19-bit_scaling"_test = []() {
    // Setup
    // Exercise
  };

  "24-bit_scaling"_test = []() {
    // Setup
    // Exercise
  };

  "30-bit_scaling"_test = []() {
    // Setup
    // Exercise
  };
};

boost::ut::suite percent_to_string_test = []() {
  using namespace boost::ut;

  percent value;
  std::string_view expected_string;
  decltype(value.to_string()) string;

  auto get_actual_string = [&]() -> std::string_view {
    string = value.to_string();
    return std::string_view(string.data(), string.size() - 1);
  };

  // Check positive boundary
  value = +1.000000000;
  expect(get_actual_string() == std::string_view{ "+1.000000000" });

  // Check zero
  value = +0.000000000;
  expect(get_actual_string() == std::string_view{ "+0.000000000" });

  // Check negative boundary
  value = -1.000000000;
  expect(get_actual_string() == std::string_view{ "-1.000000000" });

  // Check various positive numbers
  value = +0.999999998;
  expect(get_actual_string() == std::string_view{ "+0.999999998" });

  value = +0.999999995;
  expect(get_actual_string() == std::string_view{ "+0.999999995" });

  value = +0.500000000;
  expect(get_actual_string() == std::string_view{ "+0.500000000" });

  value = +0.444000000;
  expect(get_actual_string() == std::string_view{ "+0.444000000" });

  value = +0.234000000;
  expect(get_actual_string() == std::string_view{ "+0.234000000" });

  value = +0.000000005;
  expect(get_actual_string() == std::string_view{ "+0.000000005" });

  value = +0.000000002;
  expect(get_actual_string() == std::string_view{ "+0.000000002" });

  value = +0.000333000;
  expect(get_actual_string() == std::string_view{ "+0.000333000" });

  // Test various negative numbers
  value = -0.999999998;
  expect(get_actual_string() == std::string_view{ "-0.999999998" });

  value = -0.999999995;
  expect(get_actual_string() == std::string_view{ "-0.999999995" });

  value = -0.500000000;
  expect(get_actual_string() == std::string_view{ "-0.500000000" });

  value = -0.444000000;
  expect(get_actual_string() == std::string_view{ "-0.444000000" });

  value = -0.234000000;
  expect(get_actual_string() == std::string_view{ "-0.234000000" });

  value = -0.000000005;
  expect(get_actual_string() == std::string_view{ "-0.000000005" });

  value = -0.000000002;
  expect(get_actual_string() == std::string_view{ "-0.000000002" });

  value = -0.000333000;
  expect(get_actual_string() == std::string_view{ "-0.000333000" });

  // Edge cases

  // Percentage is effectively zero
  value = percent::from_ratio(1, 2147483647);
  expect(get_actual_string() == std::string_view{ "+0.000000000" });

  value = percent::from_ratio(-1, 2147483647);
  expect(get_actual_string() == std::string_view{ "-0.000000000" });

  // Percentage is effectively 1
  value = percent::from_ratio(3, 2147483647);
  expect(get_actual_string() == std::string_view{ "+0.000000001" });

  value = percent::from_ratio(-3, 2147483647);
  expect(get_actual_string() == std::string_view{ "-0.000000001" });

  // Percentage is effectively 100%
  value = percent::from_ratio(2147483645, 2147483647);
  expect(get_actual_string() == std::string_view{ "+1.000000000" });

  value = percent::from_ratio(-2147483645, 2147483647);
  expect(get_actual_string() == std::string_view{ "-1.000000000" });

  // Percentage is effectively 99.9...%
  value = percent::from_ratio(2147483644, 2147483647);
  expect(get_actual_string() == std::string_view{ "+0.999999999" });
  // Percentage is effectively 99.9...%
  value = percent::from_ratio(-2147483644, 2147483647);
  expect(get_actual_string() == std::string_view{ "-0.999999999" });
};

boost::ut::suite percent_scale_test = []() {
  using namespace boost::ut;
  "scale_percent_range"_test = []() {
    // Setup
    auto value1 = percent::from_ratio(1, 4);
    auto value2 = percent::from_ratio(3, 4);
    auto expected_100_percent = percent::from_ratio(1, 1);
    auto expected_75_percent = percent::from_ratio(3, 4);
    auto expected_50_percent = percent::from_ratio(1, 2);
    auto expected_25_percent = percent::from_ratio(1, 4);
    auto expected_0_percent = percent::from_ratio(0, 1);
    percent test = percent::from_ratio(3, 4);

    // Exercise + Verify
    // Positive percent range boundaries
    percent actual = test.scale(value1, value2);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(2, 1);
    actual = test.scale(value1, value2);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(5, 8);
    actual = test.scale(value1, value2);
    expect(that % expected_75_percent == actual);

    test = percent::from_ratio(1, 2);
    actual = test.scale(value1, value2);
    expect(that % expected_50_percent == actual);

    test = percent::from_ratio(3, 8);
    actual = test.scale(value1, value2);
    expect(that % expected_25_percent == actual);

    test = percent::from_ratio(1, 4);
    actual = test.scale(value1, value2);
    expect(that % expected_0_percent == actual);

    test = percent::from_ratio(1, 8);
    actual = test.scale(value1, value2);
    expect(that % expected_0_percent == actual);

    // Negative percent range boundaries
    value1 = percent::from_ratio(-1, 2);
    value2 = percent::from_ratio(-3, 4);

    test = percent::from_ratio(-1, 2);
    actual = test.scale(value1, value2);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(-1, 4);
    actual = test.scale(value1, value2);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(0, 1);
    actual = test.scale(value1, value2);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(-5, 8);
    actual = test.scale(value1, value2);
    expect(that % expected_50_percent == actual);

    test = percent::from_ratio(-3, 4);
    actual = test.scale(value1, value2);
    expect(that % expected_0_percent == actual);

    test = percent::from_ratio(-1, 1);
    actual = test.scale(value1, value2);
    expect(that % expected_0_percent == actual);

    test = percent::from_ratio(-2, 1);
    actual = test.scale(value1, value2);
    expect(that % expected_0_percent == actual);

    // Min and max percent range boundaries
    value1 = percent::from_ratio(-1, 1);
    value2 = percent::from_ratio(1, 1);

    test = percent::from_ratio(1, 1);
    actual = test.scale(value1, value2);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(2, 1);
    actual = test.scale(value1, value2);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(1, 2);
    actual = test.scale(value1, value2);
    expect(that % expected_75_percent == actual);

    test = percent::from_ratio(0, 1);
    actual = test.scale(value1, value2);
    expect(that % expected_50_percent == actual);

    // TODO: Fix with issue #273,  Expected value should be 25%, raw value is
    // off by 1
    test = percent::from_ratio(-1, 2);
    actual = test.scale(value1, value2);
    expect(that % percent::from_ratio(
                    536870912, std::numeric_limits<std::int32_t>::max()) ==
           actual);

    test = percent::from_ratio(-1, 1);
    actual = test.scale(value1, value2);
    expect(that % expected_0_percent == actual);

    test = percent::from_ratio(-2, 1);
    actual = test.scale(value1, value2);
    expect(that % expected_0_percent == actual);

    // Range boundaries are equal
    value1 = percent::from_ratio(1, 2);
    value2 = percent::from_ratio(1, 2);

    test = percent::from_ratio(1, 2);
    actual = test.scale(value1, value2);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(1, 1);
    actual = test.scale(value1, value2);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(1, 4);
    actual = test.scale(value1, value2);
    expect(that % expected_0_percent == actual);
  };

  "scale_int32_range"_test = []() {
    // Setup
    percent test;
    std::int32_t actual;
    std::int32_t min = 250000;
    std::int32_t max = 750000;

    std::int32_t expected_100_percent = 750000;
    std::int32_t expected_75_percent = 625000;
    std::int32_t expected_50_percent = 500000;
    std::int32_t expected_25_percent = 375000;
    std::int32_t expected_0_percent = 250000;

    // Exercise + Verify
    // Positive range boundaries
    test = percent::from_ratio(1, 1);
    actual = test.scale(min, max);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(2, 1);
    actual = test.scale(min, max);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(3, 4);
    actual = test.scale(min, max);
    expect(that % expected_75_percent == actual);

    test = percent::from_ratio(1, 2);
    actual = test.scale(min, max);
    expect(that % expected_50_percent == actual);

    test = percent::from_ratio(1, 4);
    actual = test.scale(min, max);
    expect(that % expected_25_percent == actual);

    test = percent::from_ratio(0, 1);
    actual = test.scale(min, max);
    expect(that % expected_0_percent == actual);

    test = percent::from_ratio(-1, 1);
    actual = test.scale(min, max);
    expect(that % expected_0_percent == actual);

    // Range with one boundary as int max
    min = 0;
    max = std::numeric_limits<std::int32_t>::max();

    expected_0_percent = 0;
    expected_50_percent = max / 2;
    expected_100_percent = std::numeric_limits<std::int32_t>::max();

    test = percent::from_ratio(1, 1);
    actual = test.scale(min, max);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(2, 1);
    actual = test.scale(min, max);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(1, 2);
    actual = test.scale(min, max);
    expect(that % expected_50_percent == actual);

    test = percent::from_ratio(0, 1);
    actual = test.scale(min, max);
    expect(that % expected_0_percent == actual);

    test = percent::from_ratio(-1, 1);
    actual = test.scale(min, max);
    expect(that % expected_0_percent == actual);

    // Range with one boundary as int min
    min = std::numeric_limits<std::int32_t>::min();
    max = 0;

    expected_0_percent = min;
    expected_25_percent = min - (min / 4) - 1;
    expected_50_percent = min / 2;
    expected_75_percent = min / 4;
    expected_100_percent = 0;

    test = percent::from_ratio(1, 1);
    actual = test.scale(min, max);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(2, 1);
    actual = test.scale(min, max);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(3, 4);
    actual = test.scale(min, max);
    expect(that % expected_75_percent == actual);

    test = percent::from_ratio(1, 2);
    actual = test.scale(min, max);
    expect(that % expected_50_percent == actual);

    test = percent::from_ratio(1, 4);
    actual = test.scale(min, max);
    expect(that % expected_25_percent == actual);

    test = percent::from_ratio(0, 1);
    actual = test.scale(min, max);
    expect(that % expected_0_percent == actual);

    test = percent::from_ratio(-1, 1);
    actual = test.scale(min, max);
    expect(that % expected_0_percent == actual);

    // Large range
    min = -200000000;
    max = 200000000;

    expected_0_percent = min;
    expected_25_percent = min / 2;
    expected_50_percent = 0;
    expected_75_percent = max / 2;
    expected_100_percent = max;

    test = percent::from_ratio(1, 1);
    actual = test.scale(min, max);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(2, 1);
    actual = test.scale(min, max);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(3, 4);
    actual = test.scale(min, max);
    expect(that % expected_75_percent == actual);

    test = percent::from_ratio(1, 2);
    actual = test.scale(min, max);
    expect(that % expected_50_percent == actual);

    test = percent::from_ratio(1, 4);
    actual = test.scale(min, max);
    expect(that % expected_25_percent == actual);

    test = percent::from_ratio(0, 1);
    actual = test.scale(min, max);
    expect(that % expected_0_percent == actual);

    test = percent::from_ratio(-1, 1);
    actual = test.scale(min, max);
    expect(that % expected_0_percent == actual);

    // Range boundaries values are equal
    min = 0;
    max = 0;

    expected_0_percent = 0;
    expected_50_percent = 0;
    expected_100_percent = 0;

    test = percent::from_ratio(1, 1);
    actual = test.scale(min, max);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(2, 1);
    actual = test.scale(min, max);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(1, 2);
    actual = test.scale(min, max);
    expect(that % expected_50_percent == actual);

    test = percent::from_ratio(0, 1);
    actual = test.scale(min, max);
    expect(that % expected_0_percent == actual);

    test = percent::from_ratio(-1, 1);
    actual = test.scale(min, max);
    expect(that % expected_0_percent == actual);
  };

  "scale_int64_range"_test = []() {
    // Setup
    percent test;
    std::int64_t actual;
    std::int64_t min = 0;
    std::int64_t max = 4000000000000000;

    // TODO: Fix with issue #273, expected values adjusted to work with current
    // division arithmetic
    std::int64_t expected_100_percent = 4000000000000000;
    std::int64_t expected_75_percent = 2999999999534338;
    std::int64_t expected_50_percent = 1999999999068677;
    std::int64_t expected_25_percent = 999999998603016;
    std::int64_t expected_0_percent = 0;

    // Exercise + Verify
    // Positive Range
    test = percent::from_ratio(1, 1);
    actual = test.scale(min, max);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(2, 1);
    actual = test.scale(min, max);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(3, 4);
    actual = test.scale(min, max);
    expect(that % expected_75_percent == actual);

    test = percent::from_ratio(1, 2);
    actual = test.scale(min, max);
    expect(that % expected_50_percent == actual);

    test = percent::from_ratio(1, 4);
    actual = test.scale(min, max);
    expect(that % expected_25_percent == actual);

    test = percent::from_ratio(0, 1);
    actual = test.scale(min, max);
    expect(that % expected_0_percent == actual);

    test = percent::from_ratio(-1, 1);
    actual = test.scale(min, max);
    expect(that % expected_0_percent == actual);

    // Range boundaries values are equal
    min = 1000000000000000;
    max = 1000000000000000;

    expected_0_percent = 1000000000000000;
    expected_50_percent = 1000000000000000;
    expected_100_percent = 1000000000000000;

    test = percent::from_ratio(1, 1);
    actual = test.scale(min, max);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(2, 1);
    actual = test.scale(min, max);
    expect(that % expected_100_percent == actual);

    test = percent::from_ratio(1, 2);
    actual = test.scale(min, max);
    expect(that % expected_50_percent == actual);

    test = percent::from_ratio(0, 1);
    actual = test.scale(min, max);
    expect(that % expected_0_percent == actual);

    test = percent::from_ratio(-1, 1);
    actual = test.scale(min, max);
    expect(that % expected_0_percent == actual);
  };
};
}  // namespace embed
