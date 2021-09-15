#include "full_scale.hpp"

#include <boost/ut.hpp>
#include <cstdio>

namespace {
template<size_t bit_width>
void print_bits_conversions()
{
  embed::full_scale<uint32_t> value;
  for (uint32_t i = 0; i < (1 << bit_width) + 2; i++) {
    value = embed::bit_depth<uint32_t, bit_width>(i);
    printf("%03d\t0x%08X\t%10lu\n", i, value, value);
  }
}

template<size_t bit_width>
void print_signed_bits_conversions()
{
  embed::full_scale<int32_t> value;
  constexpr int32_t max_value = 1 << (bit_width - 1);
  for (int32_t i = -max_value - 2; i < max_value + 2; i++) {
    value = embed::bit_depth<int32_t, bit_width>(i);
    printf("%03d\t0x%08X\t%11d\n", i, value, value);
  }
}

void print_nums()
{
  for (int16_t i = -256; i != +256; i++) {
    static constexpr auto mask = embed::generate_field_of_ones<9>();
    printf("%04d\t0x%03X\n", i, static_cast<uint16_t>(i & mask));
  }
}
}

using namespace boost::ut;

struct unsigned_result_t
{
  uint32_t input;
  uint32_t output;
};

suite one_bit_scaling = [] {
  // Setup
  static constexpr unsigned_result_t expected_array[] = { { 0, 0x00000000 },
                                                          { 1, 0xFFFFFFFF } };

  embed::full_scale<uint32_t> value;

  // Exercise
  for (const auto& expected_values : expected_array) {
    value = embed::bit_depth<uint32_t, 1>(expected_values.input);

    expect(that % expected_values.output == value.value());
  }
};

suite four_bit_scaling = [] {
  // Setup
  static constexpr unsigned_result_t expected_array[] = {
    { 0, 0x00000000 },  { 1, 0x11111111 },  { 2, 0x22222222 },
    { 3, 0x33333333 },  { 4, 0x44444444 },  { 5, 0x55555555 },
    { 6, 0x66666666 },  { 7, 0x77777777 },  { 8, 0x88888888 },
    { 9, 0x99999999 },  { 10, 0xAAAAAAAA }, { 11, 0xBBBBBBBB },
    { 12, 0xCCCCCCCC }, { 13, 0xDDDDDDDD }, { 14, 0xEEEEEEEE },
    { 15, 0xFFFFFFFF },
  };

  embed::full_scale<uint32_t> value;

  // Exercise
  for (const auto& expected_values : expected_array) {
    value = embed::bit_depth<uint32_t, 4>(expected_values.input);

    expect(that % expected_values.output == value.value());
  }
};

void print_int_ratio_multiply()
{
  // Setup
  static constexpr std::array<uint32_t, 6> expected_array = {
    1, 24, 126, 201, 345, 879,
  };

  embed::full_scale<uint32_t> ratio =
    embed::bit_depth<uint32_t, 32>(std::numeric_limits<uint32_t>::max() / 2);

  // Exercise
  for (const auto& expected_values : expected_array) {
    auto new_value = expected_values * ratio;
    printf("%llu) %llu/2 -> %llu\n", ratio.value(), expected_values, new_value);
  }
}

int main()
{
  // puts("\n=========== 2-bit ===========\n");

  // print_bits_conversions<2>();

  // puts("\n=========== 3-bit ===========\n");

  // print_bits_conversions<3>();

  // puts("\n=========== 3-signed-bit ===========\n");

  // print_signed_bits_conversions<3>();

  // puts("\n=========== 4-bit ===========\n");

  print_bits_conversions<4>();
  print_int_ratio_multiply();

  return 0;
}