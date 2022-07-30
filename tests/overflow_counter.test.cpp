#include <boost/ut.hpp>
#include <libembeddedhal/overflow_counter.hpp>

namespace hal {
boost::ut::suite overflow_counter_test = []() {
  using namespace boost::ut;

  "overflow_counter::update() increment by 1"_test = []() {
    overflow_counter counter;

    expect(that % 0 == counter.update(0));
    expect(that % 1 == counter.update(1));
    expect(that % 2 == counter.update(2));
    expect(that % 3 == counter.update(3));
    expect(that % 4 == counter.update(4));
    expect(that % 5 == counter.update(5));
    expect(that % 6 == counter.update(6));
  };

  // This is the case when a counter has been stopped. It shouldn't detect an
  // overflow as the value hasn't overflowed it just has not moved.
  "overflow_counter::update() increment by 0 expect no change"_test = []() {
    overflow_counter counter;

    expect(that % 10 == counter.update(10));
    expect(that % 10 == counter.update(10));
    expect(that % 10 == counter.update(10));
    expect(that % 10 == counter.update(10));
    expect(that % 10 == counter.update(10));
    expect(that % 10 == counter.update(10));
    expect(that % 10 == counter.update(10));
  };

  // This is the case when a counter has been stopped. It shouldn't detect an
  // overflow as the value hasn't overflowed it just has not moved.
  "overflow_counter::update() overflow w/ 32 bits"_test = []() {
    overflow_counter counter;

    expect(that % 0x0'0000'000A == counter.update(0xA));
    expect(that % 0x1'0000'0000 == counter.update(0x0));
    expect(that % 0x1'0000'000A == counter.update(0xA));

    expect(that % 0x2'0000'0000 == counter.update(0x0));
    expect(that % 0x2'0000'000A == counter.update(0xA));

    expect(that % 0x3'0000'0000 == counter.update(0x0));
    expect(that % 0x3'0000'000A == counter.update(0xA));

    expect(that % 0x4'0000'0000 == counter.update(0x0));
    expect(that % 0x4'0000'000A == counter.update(0xA));

    expect(that % 0x5'0000'0000 == counter.update(0x0));
    expect(that % 0x5'0000'000A == counter.update(0xA));
    expect(that % 0x5'0000'000B == counter.update(0xB));
    expect(that % 0x5'0000'000C == counter.update(0xC));
    expect(that % 0x5'0000'000D == counter.update(0xD));
    expect(that % 0x5'0000'000E == counter.update(0xE));
    expect(that % 0x5'0000'000F == counter.update(0xF));
    expect(that % 0x6'0000'0000 == counter.update(0x0));
  };

  "overflow_counter::update() overflow w/ 8 bits"_test = []() {
    overflow_counter<8> counter;

    expect(that % 0x00A == counter.update(0xA));
    expect(that % 0x100 == counter.update(0x0));
    expect(that % 0x10A == counter.update(0xA));

    expect(that % 0x200 == counter.update(0x0));
    expect(that % 0x20A == counter.update(0xA));

    expect(that % 0x300 == counter.update(0x0));
    expect(that % 0x30A == counter.update(0xA));

    expect(that % 0x400 == counter.update(0x0));
    expect(that % 0x40A == counter.update(0xA));

    expect(that % 0x500 == counter.update(0x0));
    expect(that % 0x50A == counter.update(0xA));
    expect(that % 0x50B == counter.update(0xB));
    expect(that % 0x50C == counter.update(0xC));
    expect(that % 0x50D == counter.update(0xD));
    expect(that % 0x50E == counter.update(0xE));
    expect(that % 0x50F == counter.update(0xF));
    expect(that % 0x600 == counter.update(0x0));
  };

  "overflow_counter::update() reset"_test = []() {
    overflow_counter<8> counter;

    expect(that % 0x00A == counter.update(0xA));
    expect(that % 0x100 == counter.update(0x0));
    expect(that % 0x10A == counter.update(0xA));

    expect(that % 0x200 == counter.update(0x0));
    expect(that % 0x20A == counter.update(0xA));

    expect(that % 0x300 == counter.update(0x0));
    expect(that % 0x30A == counter.update(0xA));

    expect(that % 0x400 == counter.update(0x0));
    expect(that % 0x40A == counter.update(0xA));

    counter.reset();

    expect(that % 0x00A == counter.update(0xA));
  };
};
}  // namespace hal
