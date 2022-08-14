#include <boost/ut.hpp>
#include <libhal/serial/util.hpp>
#include <span>

#include "ostreams.hpp"

namespace hal {
boost::ut::suite serial_util_test = []() {
  using namespace boost::ut;
  using namespace std::chrono_literals;

  static constexpr hal::byte write_failure_byte{ 'C' };
  static constexpr hal::byte filler_byte{ 'A' };

  class fake_serial : public hal::serial
  {
  public:
    [[nodiscard]] status driver_configure(const settings&) noexcept override
    {
      return {};
    }
    [[nodiscard]] status driver_write(
      std::span<const hal::byte> p_data) noexcept override
    {
      if (p_data[0] == write_failure_byte) {
        return hal::new_error();
      }
      m_out = p_data;
      return {};
    }

    [[nodiscard]] bytes_available_t driver_bytes_available() noexcept override
    {
      return bytes_available_t{ .available = 0, .capacity = 1000UL };
    }

    [[nodiscard]] result<std::span<hal::byte>> driver_read(
      std::span<hal::byte> p_data) noexcept override
    {
      read_was_called = true;
      if (m_read_fails) {
        return hal::new_error();
      }
      // only fill 1 byte at a time
      p_data[0] = filler_byte;
      p_data = p_data.subspan(0, 1);
      return p_data;
    }

    [[nodiscard]] status driver_flush() noexcept override
    {
      flush_called = true;
      return {};
    }

    virtual ~fake_serial()
    {
    }

    std::span<const hal::byte> m_out{};
    bool read_was_called = false;
    bool flush_called = false;
    bool m_read_fails = false;
  };

  "serial/util"_test = []() {
    "[success] write"_test = []() {
      // Setup
      fake_serial serial;
      const std::array<hal::byte, 4> expected_payload{};

      // Exercise
      auto result = write(serial, expected_payload);

      // Verify
      expect(bool{ result });
      expect(!serial.flush_called);
      expect(that % expected_payload.data() == serial.m_out.data());
      expect(that % expected_payload.size() == serial.m_out.size());
      expect(that % !serial.read_was_called);
    };

    "[failure] write"_test = []() {
      // Setup
      fake_serial serial;
      const std::array<hal::byte, 4> expected_payload{ write_failure_byte };

      // Exercise
      auto result = write(serial, expected_payload);

      // Verify
      expect(!bool{ result });
      expect(!serial.flush_called);
      expect(that % nullptr == serial.m_out.data());
      expect(that % 0 == serial.m_out.size());
      expect(that % !serial.read_was_called);
    };

    "[success] read"_test = []() {
      // Setup
      fake_serial serial;
      std::array<hal::byte, 4> expected_buffer;
      expected_buffer.fill(filler_byte);
      std::array<hal::byte, 4> actual_buffer;

      // Exercise
      auto result = read(serial, actual_buffer, never_timeout());

      // Verify
      expect(result.has_value());
      expect(!serial.flush_called);
      expect(that % nullptr == serial.m_out.data());
      expect(that % 0 == serial.m_out.size());
      bool test = expected_buffer == result.value();
      expect(that % test);
    };

    "[failure read] read"_test = []() {
      // Setup
      fake_serial serial;
      std::array<hal::byte, 4> expected_buffer;
      serial.m_read_fails = true;

      // Exercise
      auto result = read(serial, expected_buffer, never_timeout());

      // Verify
      expect(!result.has_value());
      expect(!serial.flush_called);
      expect(that % serial.read_was_called);
      expect(that % nullptr == serial.m_out.data());
      expect(that % 0 == serial.m_out.size());
    };

    "[success] read<Length>"_test = []() {
      // Setup
      fake_serial serial;
      std::array<hal::byte, 5> expected_buffer;
      expected_buffer.fill(filler_byte);

      // Exercise
      auto result = read<expected_buffer.size()>(serial, never_timeout());

      // Verify
      expect(result.has_value());
      expect(!serial.flush_called);
      expect(that % expected_buffer == result.value());
      expect(that % serial.read_was_called);
      expect(that % nullptr == serial.m_out.data());
      expect(that % 0 == serial.m_out.size());
    };

    "[failure read] read<Length>"_test = []() {
      // Setup
      fake_serial serial;
      serial.m_read_fails = true;

      // Exercise
      auto result = read<5>(serial, never_timeout());

      // Verify
      expect(!result.has_value());
      expect(!serial.flush_called);
      expect(that % serial.read_was_called);
      expect(that % nullptr == serial.m_out.data());
      expect(that % 0 == serial.m_out.size());
    };

    "[success] write_then_read"_test = []() {
      // Setup
      fake_serial serial;
      const std::array<hal::byte, 4> expected_payload{};
      std::array<hal::byte, 4> expected_buffer;
      expected_buffer.fill(filler_byte);
      std::array<hal::byte, 4> actual_buffer;

      // Exercise
      auto result = write_then_read(
        serial, expected_payload, actual_buffer, never_timeout());

      // Verify
      expect(result.has_value());
      expect(!serial.flush_called);
      expect(that % expected_payload.data() == serial.m_out.data());
      expect(that % expected_payload.size() == serial.m_out.size());
      expect(that % expected_buffer == actual_buffer);
    };

    "[failure read] write_then_read"_test = []() {
      // Setup
      fake_serial serial;
      const std::array<hal::byte, 4> expected_payload{};
      std::array<hal::byte, 4> expected_buffer;
      expected_buffer.fill(filler_byte);
      std::array<hal::byte, 4> actual_buffer;
      serial.m_read_fails = true;

      // Exercise
      auto result = write_then_read(
        serial, expected_payload, actual_buffer, never_timeout());

      // Verify
      expect(!result.has_value());
      expect(!serial.flush_called);
      expect(that % serial.read_was_called);
      expect(that % expected_payload.data() == serial.m_out.data());
      expect(that % expected_payload.size() == serial.m_out.size());
      expect(that % expected_buffer != actual_buffer);
    };

    "[failure on write] write_then_read"_test = []() {
      // Setup
      fake_serial serial;
      const std::array<hal::byte, 4> expected_payload{ write_failure_byte };
      std::array<hal::byte, 4> expected_buffer;

      // Exercise
      auto result = write_then_read(
        serial, expected_payload, expected_buffer, never_timeout());

      // Verify
      expect(!result.has_value());
      expect(!serial.flush_called);
      expect(that % !serial.read_was_called);
      expect(that % nullptr == serial.m_out.data());
      expect(that % 0 == serial.m_out.size());
    };

    "[success] write_then_read<Length>"_test = []() {
      // Setup
      fake_serial serial;
      const std::array<hal::byte, 4> expected_payload{};
      std::array<hal::byte, 5> expected_buffer;
      expected_buffer.fill(filler_byte);

      // Exercise
      auto result =
        write_then_read<5>(serial, expected_payload, never_timeout());

      auto actual_array = result.value();

      // Verify
      expect(result.has_value());
      expect(!serial.flush_called);
      expect(that % expected_payload.data() == serial.m_out.data());
      expect(that % expected_payload.size() == serial.m_out.size());
      expect(serial.read_was_called);
      expect(that % expected_buffer == actual_array);
    };

    "[failure on write] write_then_read<Length>"_test = []() {
      // Setup
      fake_serial serial;
      const std::array<hal::byte, 4> expected_payload{ write_failure_byte };

      // Exercise
      auto result =
        write_then_read<5>(serial, expected_payload, never_timeout());

      // Verify
      expect(!result.has_value());
      expect(!serial.flush_called);
      expect(that % !serial.read_was_called);
      expect(that % nullptr == serial.m_out.data());
      expect(that % 0 == serial.m_out.size());
    };

    "[failure read] write_then_read<Length>"_test = []() {
      // Setup
      fake_serial serial;
      const std::array<hal::byte, 4> expected_payload{};
      serial.m_read_fails = true;

      // Exercise
      auto result =
        write_then_read<5>(serial, expected_payload, never_timeout());

      // Verify
      expect(!result.has_value());
      expect(!serial.flush_called);
      expect(that % serial.read_was_called);
      expect(that % expected_payload.data() == serial.m_out.data());
      expect(that % expected_payload.size() == serial.m_out.size());
    };
  };
};
}  // namespace hal
