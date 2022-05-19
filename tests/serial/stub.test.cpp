#include <boost/ut.hpp>
#include <libembeddedhal/serial/stub.hpp>
#include <span>

namespace embed {
boost::ut::suite serial_stub_test = []() {
  using namespace boost::ut;

  "embed::stub::serial::configure()"_test = []() {
    // Setup
    embed::stub::serial stub;

    // Exercise + Verify
    expect(nothrow([&stub]() { auto stub_result = stub.configure({}); }));
    expect(bool{ stub.configure({}) });
  };

  "embed::stub::serial::write()"_test = []() {
    // Setup
    embed::stub::serial stub;

    // Exercise + Verify
    expect(nothrow([&stub]() {
      std::array<std::byte, 15> buffer{};
      expect(bool{ stub.write(buffer) });
    }));
  };

  "embed::stub::serial::read()"_test = []() {
    // Setup
    embed::stub::serial stub;

    // Exercise + Verify
    expect(nothrow([&stub]() {
      std::array<std::byte, 15> buffer{};
      auto stub_result = stub.read(buffer);
      expect(that % buffer.data() == stub_result.value().data());
      expect(that % 0 == stub_result.value().size());
    }));
  };

  "embed::stub::serial::bytes_available()"_test = []() {
    // Setup
    embed::stub::serial stub;

    // Exercise + Verify
    expect(nothrow(
      [&stub]() { expect(that % 0 == stub.bytes_available().value()); }));
  };

  "embed::stub::serial::flush()"_test = []() {
    // Setup
    embed::stub::serial stub;

    // Exercise + Verify
    expect(nothrow([&stub]() { expect(bool{ stub.flush() }); }));
  };
};
}  // namespace embed
