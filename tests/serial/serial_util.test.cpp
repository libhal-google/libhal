#include <boost/ut.hpp>
#include <libembeddedhal/serial/serial_util.hpp>
#include <span>

namespace embed {
boost::ut::suite serial_util_test = []() {
  using namespace boost::ut;
  using namespace std::chrono_literals;

  // static constexpr std::byte success_filler{ 0xF5 };
  static constexpr std::byte write_failure_byte{ 0x33 };
  static constexpr std::byte filler_byte{ 0xA5 };
  static constexpr std::chrono::nanoseconds expected_timeout = 1ms;
  // Buffer must be large enough to cause a timeout of 0ns to actually fire on
  // host tests.
  static constexpr size_t buffer_size_for_timeout = 1024;

  class dummy : public embed::serial
  {
  public:
    [[nodiscard]] boost::leaf::result<void> driver_configure(
      const settings&) noexcept override
    {
      return {};
    }
    [[nodiscard]] boost::leaf::result<void> driver_write(
      std::span<const std::byte> p_data) noexcept override
    {
      if (p_data[0] == write_failure_byte) {
        return boost::leaf::new_error();
      }
      m_out = p_data;
      return {};
    }

    [[nodiscard]] boost::leaf::result<size_t> driver_bytes_available() noexcept
      override
    {
      if (m_bytes_available_fails) {
        return boost::leaf::new_error();
      }
      return ++m_bytes_available;
    }

    [[nodiscard]] boost::leaf::result<std::span<const std::byte>> driver_read(
      std::span<std::byte> p_data) noexcept override
    {
      if (m_read_fails) {
        return boost::leaf::new_error();
      }
      m_in = p_data;
      std::fill(p_data.begin(), p_data.end(), filler_byte);
      return p_data;
    }

    [[nodiscard]] boost::leaf::result<void> driver_flush() noexcept override
    {
      m_flush_called = true;
      return {};
    }

    virtual ~dummy() {}

    std::span<const std::byte> m_out{};
    std::span<std::byte> m_in{};
    size_t m_bytes_available = 0;
    bool m_flush_called = false;
    bool m_bytes_available_fails = false;
    bool m_read_fails = false;
  };

  "[success] write"_test = []() {
    // Setup
    dummy serial;
    const std::array<std::byte, 4> expected_payload{};

    // Exercise
    auto result = write(serial, expected_payload);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(successful);
    expect(!serial.m_flush_called);
    expect(that % expected_payload.data() == serial.m_out.data());
    expect(that % expected_payload.size() == serial.m_out.size());
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % 0 == serial.m_bytes_available);
  };

  "[failure] write"_test = []() {
    // Setup
    dummy serial;
    const std::array<std::byte, 4> expected_payload{ write_failure_byte };

    // Exercise
    auto result = write(serial, expected_payload);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_out.data());
    expect(that % 0 == serial.m_out.size());
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % 0 == serial.m_bytes_available);
  };

  "[success] read"_test = []() {
    // Setup
    dummy serial;
    std::array<std::byte, 4> expected_buffer;

    // Exercise
    auto result = read(serial, expected_buffer, expected_timeout);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(successful);
    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_out.data());
    expect(that % 0 == serial.m_out.size());
    expect(that % expected_buffer.data() == serial.m_in.data());
    expect(that % expected_buffer.size() == serial.m_in.size());
    expect(that % 4 == serial.m_bytes_available);
  };

  "[failure bytes_available] read"_test = []() {
    // Setup
    dummy serial;
    std::array<std::byte, 4> expected_buffer;
    serial.m_bytes_available_fails = true;

    // Exercise
    auto result = read(serial, expected_buffer, expected_timeout);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % nullptr == serial.m_out.data());
    expect(that % 0 == serial.m_out.size());
    expect(that % 0 == serial.m_bytes_available);
  };

  "[failure read] read"_test = []() {
    // Setup
    dummy serial;
    std::array<std::byte, 4> expected_buffer;
    serial.m_read_fails = true;

    // Exercise
    auto result = read(serial, expected_buffer, expected_timeout);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % nullptr == serial.m_out.data());
    expect(that % 0 == serial.m_out.size());
    expect(that % 4 == serial.m_bytes_available);
  };

  "[success] read<Length>"_test = []() {
    // Setup
    dummy serial;
    std::array<std::byte, 5> expected_buffer;
    expected_buffer.fill(filler_byte);

    // Exercise
    auto result = read<expected_buffer.size()>(serial, expected_timeout);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(successful);
    expect(!serial.m_flush_called);
    expect(std::equal(
      expected_buffer.begin(), expected_buffer.end(), result.value().begin()));
    expect(that % nullptr == serial.m_out.data());
    expect(that % 0 == serial.m_out.size());
  };

  "[failure bytes_available] read<Length>"_test = []() {
    // Setup
    dummy serial;
    serial.m_bytes_available_fails = true;

    // Exercise
    auto result = read<5>(serial);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % nullptr == serial.m_out.data());
    expect(that % 0 == serial.m_out.size());
    expect(that % 0 == serial.m_bytes_available);
  };

  "[failure read] read<Length>"_test = []() {
    // Setup
    dummy serial;
    serial.m_read_fails = true;

    // Exercise
    auto result = read<5>(serial);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % nullptr == serial.m_out.data());
    expect(that % 0 == serial.m_out.size());
    expect(that % 5 == serial.m_bytes_available);
  };

  "[success] write_then_read"_test = []() {
    // Setup
    dummy serial;
    const std::array<std::byte, 4> expected_payload{};
    std::array<std::byte, 4> expected_buffer;

    // Exercise
    auto result = write_then_read(
      serial, expected_payload, expected_buffer, expected_timeout);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(successful);
    expect(!serial.m_flush_called);
    expect(that % expected_payload.data() == serial.m_out.data());
    expect(that % expected_payload.size() == serial.m_out.size());
    expect(that % expected_buffer.data() == serial.m_in.data());
    expect(that % expected_buffer.size() == serial.m_in.size());
  };

  "[failure bytes_available] write_then_read"_test = []() {
    // Setup
    dummy serial;
    const std::array<std::byte, 4> expected_payload{};
    std::array<std::byte, 4> expected_buffer;
    expected_buffer.fill(filler_byte);
    serial.m_bytes_available_fails = true;

    // Exercise
    auto result = write_then_read(serial, expected_payload, expected_buffer);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % expected_payload.data() == serial.m_out.data());
    expect(that % expected_payload.size() == serial.m_out.size());
    expect(that % 0 == serial.m_bytes_available);
  };

  "[failure read] write_then_read"_test = []() {
    // Setup
    dummy serial;
    const std::array<std::byte, 4> expected_payload{};
    std::array<std::byte, 4> expected_buffer;
    expected_buffer.fill(filler_byte);
    serial.m_read_fails = true;

    // Exercise
    auto result = write_then_read(serial, expected_payload, expected_buffer);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % expected_payload.data() == serial.m_out.data());
    expect(that % expected_payload.size() == serial.m_out.size());
    expect(that % 4 == serial.m_bytes_available);
  };

  "[failure on timeout] read"_test = []() {
    // Setup
    dummy serial;

    // Exercise
    boost::leaf::try_handle_all(
      [&]() -> boost::leaf::result<void> {
        std::array<std::byte, buffer_size_for_timeout> expected_buffer;
        BOOST_LEAF_CHECK(read(serial, expected_buffer, 0ns));
        return {};
      },
      []([[maybe_unused]] embed::error::timeout p_timeout) { expect(true); },
      []() {
        expect(false) << "Uncaught error thrown by write_then_read<5>()";
      });

    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % nullptr == serial.m_out.data());
    expect(that % 0 == serial.m_out.size());
    // bytes_available must be called at least once before checking if the
    // operation timed out, thus m_bytes_available must be non-zero.
    expect(that % 0 != serial.m_bytes_available);
  };

  "[failure on write] write_then_read"_test = []() {
    // Setup
    dummy serial;
    const std::array<std::byte, 4> expected_payload{ write_failure_byte };
    std::array<std::byte, 4> expected_buffer;

    // Exercise
    auto result = write_then_read(serial, expected_payload, expected_buffer);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % nullptr == serial.m_out.data());
    expect(that % 0 == serial.m_out.size());
    expect(that % 0 == serial.m_bytes_available);
  };

  "[failure on timeout] write_then_read"_test = []() {
    // Setup
    dummy serial;
    const std::array<std::byte, 4> expected_payload{};

    // Exercise
    boost::leaf::try_handle_all(
      [&]() -> boost::leaf::result<void> {
        std::array<std::byte, buffer_size_for_timeout> expected_buffer;
        BOOST_LEAF_CHECK(
          write_then_read(serial, expected_payload, expected_buffer, 0ns));
        return {};
      },
      []([[maybe_unused]] embed::error::timeout p_timeout) { expect(true); },
      []() {
        expect(false) << "Uncaught error thrown by write_then_read<5>()";
      });

    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % expected_payload.data() == serial.m_out.data());
    expect(that % expected_payload.size() == serial.m_out.size());
    // bytes_available must be called at least once before checking if the
    // operation timed out, thus m_bytes_available must be non-zero.
    expect(that % 0 != serial.m_bytes_available);
  };

  "[success] write_then_read<Length>"_test = []() {
    // Setup
    dummy serial;
    const std::array<std::byte, 4> expected_payload{};
    std::array<std::byte, 4> expected_buffer{};
    expected_buffer.fill(filler_byte);

    // Exercise
    auto result =
      write_then_read<5>(serial, expected_payload, expected_timeout);
    bool successful = static_cast<bool>(result);
    auto actual_array = result.value();

    // Verify
    expect(successful);
    expect(!serial.m_flush_called);
    expect(that % expected_payload.data() == serial.m_out.data());
    expect(that % expected_payload.size() == serial.m_out.size());
    expect(std::equal(
      expected_buffer.begin(), expected_buffer.end(), actual_array.begin()));
  };

  "[failure on write] write_then_read<Length>"_test = []() {
    // Setup
    dummy serial;
    const std::array<std::byte, 4> expected_payload{ write_failure_byte };

    // Exercise
    auto result =
      write_then_read<5>(serial, expected_payload, expected_timeout);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % nullptr == serial.m_out.data());
    expect(that % 0 == serial.m_out.size());
    expect(that % 0 == serial.m_bytes_available);
  };

  "[failure bytes_available] write_then_read<Length>"_test = []() {
    // Setup
    dummy serial;
    const std::array<std::byte, 4> expected_payload{};
    serial.m_bytes_available_fails = true;

    // Exercise
    auto result =
      write_then_read<5>(serial, expected_payload, expected_timeout);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % expected_payload.data() == serial.m_out.data());
    expect(that % expected_payload.size() == serial.m_out.size());
    expect(that % 0 == serial.m_bytes_available);
  };

  "[failure read] write_then_read<Length>"_test = []() {
    // Setup
    dummy serial;
    const std::array<std::byte, 4> expected_payload{};
    serial.m_read_fails = true;

    // Exercise
    auto result =
      write_then_read<5>(serial, expected_payload, expected_timeout);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % expected_payload.data() == serial.m_out.data());
    expect(that % expected_payload.size() == serial.m_out.size());
    expect(that % 5 == serial.m_bytes_available);
  };

  "[failure on timeout] write_then_read<Length>"_test = []() {
    // Setup
    dummy serial;
    const std::array<std::byte, 4> expected_payload{};

    // Exercise
    boost::leaf::try_handle_all(
      [&]() -> boost::leaf::result<void> {
        BOOST_LEAF_CHECK(write_then_read<buffer_size_for_timeout>(
          serial, expected_payload, 0ns));
        return {};
      },
      []([[maybe_unused]] embed::error::timeout p_timeout) { expect(true); },
      []() {
        expect(false) << "Uncaught error thrown by write_then_read<5>()";
      });

    expect(!serial.m_flush_called);
    expect(that % nullptr == serial.m_in.data());
    expect(that % 0 == serial.m_in.size());
    expect(that % expected_payload.data() == serial.m_out.data());
    expect(that % expected_payload.size() == serial.m_out.size());
    // bytes_available must be called at least once before checking if the
    // operation timed out, thus m_bytes_available must be non-zero.
    expect(that % 0 != serial.m_bytes_available);
  };
};
}  // namespace embed
