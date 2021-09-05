#include "full_scale.hpp"

#include <cstdio>
#include <boost/ut.hpp>

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

int main()
{
  puts("\n=========== 2-bit ===========\n");

  print_bits_conversions<2>();

  puts("\n=========== 3-bit ===========\n");

  print_bits_conversions<3>();

  puts("\n=========== 3-signed-bit ===========\n");

  print_signed_bits_conversions<3>();

  puts("\n=========== 4-bit ===========\n");

  print_bits_conversions<4>();

  using namespace boost::ut;
  expect(1_i == 2);

  return 0;
}