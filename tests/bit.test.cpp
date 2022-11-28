#include <libhal/bit.hpp>

#include <boost/ut.hpp>

namespace hal {
boost::ut::suite bit_test = []() {
  using namespace boost::ut;

  "hal::bit standard usage"_test = []() {
    // Setup
    volatile std::uint32_t control_register = 1 << 15 | 1 << 16;
    constexpr auto enable_bit = bit::mask::from<1>();
    constexpr auto high_power_mode = bit::mask::from<15>();
    constexpr auto clock_divider = bit::mask::from<20, 23>();
    constexpr auto extractor_mask = bit::mask::from<16, 23>();
    constexpr auto single_bit_mask = bit::mask::from<1>();

    // Exercise
    bit::modify(control_register)
      .set<enable_bit>()
      .clear<high_power_mode>()
      .insert<clock_divider>(0xAU);
    auto extracted = bit::extract<extractor_mask>(control_register);
    auto probed = bit::extract<single_bit_mask>(control_register);
    auto probed_inline =
      bit::extract<bit::mask{ .position = 15, .width = 1 }>(control_register);

    // Verify
    expect(that % 0x00A1'0002 == control_register);
    expect(that % 0xA1 == extracted);
    expect(that % 1 == probed);
    expect(that % 0 == probed_inline);
  };
};
}
