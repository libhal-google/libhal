#include <boost/ut.hpp>
#include <libembeddedhal/full_scale.hpp>

namespace embed {
boost::ut::suite full_scale_unsigned_test = []() {
  using namespace boost::ut;

  "1-bit_scaling"_test = []() {
    // Setup
    embed::full_scale<uint32_t> value;

    // Exercise
    value = embed::bit_depth<uint32_t, 1>(0);
    expect(that % 0x0000'0000 == value.value());

    value = embed::bit_depth<uint32_t, 1>(1);
    expect(that % 0xFFFF'FFFF == value.value());
  };

  "4-bit_scaling"_test = []() {
    // Setup
    embed::full_scale<uint32_t> value;

    // Exercise

    value = embed::bit_depth<uint32_t, 4>(0);
    expect(that % 0x00000000 == value.value());

    value = embed::bit_depth<uint32_t, 4>(1);
    expect(that % 0x11111111 == value.value());

    value = embed::bit_depth<uint32_t, 4>(2);
    expect(that % 0x22222222 == value.value());

    value = embed::bit_depth<uint32_t, 4>(3);
    expect(that % 0x33333333 == value.value());

    value = embed::bit_depth<uint32_t, 4>(4);
    expect(that % 0x44444444 == value.value());

    value = embed::bit_depth<uint32_t, 4>(5);
    expect(that % 0x55555555 == value.value());

    value = embed::bit_depth<uint32_t, 4>(6);
    expect(that % 0x66666666 == value.value());

    value = embed::bit_depth<uint32_t, 4>(7);
    expect(that % 0x77777777 == value.value());

    value = embed::bit_depth<uint32_t, 4>(8);
    expect(that % 0x88888888 == value.value());

    value = embed::bit_depth<uint32_t, 4>(9);
    expect(that % 0x99999999 == value.value());

    value = embed::bit_depth<uint32_t, 4>(10);
    expect(that % 0xAAAAAAAA == value.value());

    value = embed::bit_depth<uint32_t, 4>(11);
    expect(that % 0xBBBBBBBB == value.value());

    value = embed::bit_depth<uint32_t, 4>(12);
    expect(that % 0xCCCCCCCC == value.value());

    value = embed::bit_depth<uint32_t, 4>(13);
    expect(that % 0xDDDDDDDD == value.value());

    value = embed::bit_depth<uint32_t, 4>(14);
    expect(that % 0xEEEEEEEE == value.value());

    value = embed::bit_depth<uint32_t, 4>(15);
    expect(that % 0xFFFFFFFF == value.value());
  };

  "8-bit_scaling"_test = []() {
    // Setup
    embed::full_scale<uint32_t> value;

    // Exercise
    value = embed::bit_depth<uint32_t, 8>(0x00);
    expect(that % 0x00000000 == value.value());

    value = embed::bit_depth<uint32_t, 8>(0x11);
    expect(that % 0x11111111 == value.value());

    value = embed::bit_depth<uint32_t, 8>(0x22);
    expect(that % 0x22222222 == value.value());

    value = embed::bit_depth<uint32_t, 8>(0x3A);
    expect(that % 0x3A3A3A3A == value.value());

    value = embed::bit_depth<uint32_t, 8>(0x4B);
    expect(that % 0x4B4B4B4B == value.value());

    value = embed::bit_depth<uint32_t, 8>(0x5C);
    expect(that % 0x5C5C5C5C == value.value());

    value = embed::bit_depth<uint32_t, 8>(0x6D);
    expect(that % 0x6D6D6D6D == value.value());

    value = embed::bit_depth<uint32_t, 8>(0x7E);
    expect(that % 0x7E7E7E7E == value.value());

    value = embed::bit_depth<uint32_t, 8>(0x8F);
    expect(that % 0x8F8F8F8F == value.value());

    value = embed::bit_depth<uint32_t, 8>(0xA9);
    expect(that % 0xA9A9A9A9 == value.value());

    value = embed::bit_depth<uint32_t, 8>(0xBA);
    expect(that % 0xBABABABA == value.value());

    value = embed::bit_depth<uint32_t, 8>(0xCB);
    expect(that % 0xCBCBCBCB == value.value());

    value = embed::bit_depth<uint32_t, 8>(0xDC);
    expect(that % 0xDCDCDCDC == value.value());

    value = embed::bit_depth<uint32_t, 8>(0xED);
    expect(that % 0xEDEDEDED == value.value());

    value = embed::bit_depth<uint32_t, 8>(0xFE);
    expect(that % 0xFEFEFEFE == value.value());

    value = embed::bit_depth<uint32_t, 8>(0xFF);
    expect(that % 0xFFFFFFFF == value.value());
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
    // Exercise
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

boost::ut::suite full_scale_signed_test = []() {
  using namespace boost::ut;

  "1-bit_scaling"_test = []() {
    // Setup
    embed::full_scale<int32_t> value;

    // Exercise
    value = embed::bit_depth<int32_t, 2>(0);
    expect(that % 0x0000'0000 == value.value());

    value = embed::bit_depth<int32_t, 2>(1);
    expect(that % 0x7FFF'FFFF == value.value());

    value = embed::bit_depth<int32_t, 2>(-1);
    expect(that % 0xC000'0000 == value.value());
  };

  "4-bit_scaling"_test = []() {
    // Setup
    embed::full_scale<int32_t> value;

    // Exercise
    value = embed::bit_depth<int32_t, 4>(0);
    expect(that % 0 == value.value());

    value = embed::bit_depth<int32_t, 4>(1);
    expect(that % 0b0001'0010'0100'1001'0010'0100'1001'0010 == value.value());

    value = embed::bit_depth<int32_t, 4>(2);
    expect(that % 0b0010'0100'1001'0010'0100'1001'0010'0100 == value.value());

    value = embed::bit_depth<int32_t, 4>(3);
    expect(that % 0b0011'0110'1101'1011'0110'1101'1011'0110 == value.value());

    value = embed::bit_depth<int32_t, 4>(4);
    expect(that % 0b0100'1001'0010'0100'1001'0010'0100'1001 == value.value());

    value = embed::bit_depth<int32_t, 4>(5);
    expect(that % 0b0101'1011'0110'1101'1011'0110'1101'1011 == value.value());

    value = embed::bit_depth<int32_t, 4>(6);
    expect(that % 0b0110'1101'1011'0110'1101'1011'0110'1101 == value.value());

    value = embed::bit_depth<int32_t, 4>(7);
    expect(that % 0b0111'1111'1111'1111'1111'1111'1111'1111 == value.value());

    value = embed::bit_depth<int32_t, 4>(-1);
    expect(that %
             static_cast<int32_t>(0b1111'0000'0000'0000'0000'0000'0000'0000) ==
           value.value());

    value = embed::bit_depth<int32_t, 4>(-2);
    expect(that %
             static_cast<int32_t>(0b1110'0000'0000'0000'0000'0000'0000'0000) ==
           value.value());

    value = embed::bit_depth<int32_t, 4>(-3);
    expect(that %
             static_cast<int32_t>(0b1101'0000'0000'0000'0000'0000'0000'0000) ==
           value.value());

    value = embed::bit_depth<int32_t, 4>(-4);
    expect(that %
             static_cast<int32_t>(0b1100'0000'0000'0000'0000'0000'0000'0000) ==
           value.value());

    value = embed::bit_depth<int32_t, 4>(-5);
    expect(that %
             static_cast<int32_t>(0b1011'0000'0000'0000'0000'0000'0000'0000) ==
           value.value());

    value = embed::bit_depth<int32_t, 4>(-6);
    expect(that %
             static_cast<int32_t>(0b1010'0000'0000'0000'0000'0000'0000'0000) ==
           value.value());

    value = embed::bit_depth<int32_t, 4>(-7);
    expect(that %
             static_cast<int32_t>(0b1001'0000'0000'0000'0000'0000'0000'0000) ==
           value.value());

    value = embed::bit_depth<int32_t, 4>(-8);
    expect(that %
             static_cast<int32_t>(0b1000'0000'0000'0000'0000'0000'0000'0000) ==
           value.value());
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
    // Exercise
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
}  // namespace embed
