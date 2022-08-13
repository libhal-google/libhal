#include <boost/ut.hpp>
#include <libhal/spi/util.hpp>
#include <span>

namespace hal {
boost::ut::suite spi_util_test = []() {
  using namespace boost::ut;

  static constexpr hal::byte success_filler{ 0xF5 };
  static constexpr hal::byte failure_filler{ 0x33 };
  static constexpr hal::byte filler_byte{ 0xA5 };

  class dummy : public hal::spi
  {
  public:
    [[nodiscard]] status driver_configure(const settings&) noexcept override
    {
      return {};
    }
    [[nodiscard]] status driver_transfer(std::span<const hal::byte> p_out,
                                         std::span<hal::byte> p_in,
                                         hal::byte p_filler) noexcept override
    {
      if (!p_out.empty()) {
        m_out = p_out;
      }

      if (!p_in.empty()) {
        m_in = p_in;
        std::fill(m_in.begin(), m_in.end(), filler_byte);
      }

      m_filler = p_filler;

      if (p_filler == failure_filler) {
        return hal::new_error();
      }

      return {};
    }

    virtual ~dummy()
    {
    }

    std::span<const hal::byte> m_out = std::span<const hal::byte>{};
    std::span<hal::byte> m_in = std::span<hal::byte>{};
    hal::byte m_filler = hal::byte{ 0xFF };
  };

  "[success] write"_test = []() {
    // Setup
    dummy spi;
    const std::array<hal::byte, 4> expected_payload{};

    // Exercise
    auto result = write(spi, expected_payload);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(successful);
    expect(that % expected_payload.data() == spi.m_out.data());
    expect(that % expected_payload.size() == spi.m_out.size());
    expect(that % nullptr == spi.m_in.data());
    expect(that % 0 == spi.m_in.size());
  };

  "[success] read"_test = []() {
    // Setup
    dummy spi;
    std::array<hal::byte, 4> expected_buffer;

    // Exercise
    auto result = read(spi, expected_buffer, success_filler);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(successful);
    expect(success_filler == spi.m_filler);
    expect(that % expected_buffer.data() == spi.m_in.data());
    expect(that % expected_buffer.size() == spi.m_in.size());
    expect(that % nullptr == spi.m_out.data());
    expect(that % 0 == spi.m_out.size());
  };

  "[failure] read"_test = []() {
    // Setup
    dummy spi;
    std::array<hal::byte, 4> expected_buffer;

    // Exercise
    auto result = read(spi, expected_buffer, failure_filler);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(failure_filler == spi.m_filler);
    expect(that % expected_buffer.data() == spi.m_in.data());
    expect(that % expected_buffer.size() == spi.m_in.size());
    expect(that % nullptr == spi.m_out.data());
    expect(that % 0 == spi.m_out.size());
  };

  "[success] read<Length>"_test = []() {
    // Setup
    dummy spi;
    std::array<hal::byte, 5> expected_buffer;
    expected_buffer.fill(filler_byte);

    // Exercise
    auto result = read<expected_buffer.size()>(spi, success_filler);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(successful);
    expect(success_filler == spi.m_filler);
    expect(std::equal(
      expected_buffer.begin(), expected_buffer.end(), result.value().begin()));
    expect(that % nullptr == spi.m_out.data());
    expect(that % 0 == spi.m_out.size());
  };

  "[failure] read<Length>"_test = []() {
    // Setup
    dummy spi;

    // Exercise
    auto result = read<5>(spi, failure_filler);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(failure_filler == spi.m_filler);
    expect(that % nullptr == spi.m_out.data());
    expect(that % 0 == spi.m_out.size());
  };

  "[success] write_then_read"_test = []() {
    // Setup
    dummy spi;
    const std::array<hal::byte, 4> expected_payload{};
    std::array<hal::byte, 4> expected_buffer;

    // Exercise
    auto result =
      write_then_read(spi, expected_payload, expected_buffer, success_filler);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(successful);
    expect(success_filler == spi.m_filler);
    expect(that % expected_payload.data() == spi.m_out.data());
    expect(that % expected_payload.size() == spi.m_out.size());
    expect(that % expected_buffer.data() == spi.m_in.data());
    expect(that % expected_buffer.size() == spi.m_in.size());
  };

  "[failure] write_then_read"_test = []() {
    // Setup
    dummy spi;
    const std::array<hal::byte, 4> expected_payload{};
    std::array<hal::byte, 4> expected_buffer;
    expected_buffer.fill(filler_byte);

    // Exercise
    auto result =
      write_then_read(spi, expected_payload, expected_buffer, failure_filler);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(failure_filler == spi.m_filler);
    expect(that % expected_payload.data() == spi.m_out.data());
    expect(that % expected_payload.size() == spi.m_out.size());
    expect(that % expected_buffer.data() == spi.m_in.data());
    expect(that % expected_buffer.size() == spi.m_in.size());
  };

  "[success] write_then_read<Length>"_test = []() {
    // Setup
    dummy spi;
    const std::array<hal::byte, 4> expected_payload{};
    std::array<hal::byte, 4> expected_buffer{};
    expected_buffer.fill(filler_byte);

    // Exercise
    auto result = write_then_read<5>(spi, expected_payload, success_filler);
    bool successful = static_cast<bool>(result);
    auto actual_array = result.value();

    // Verify
    expect(successful);
    expect(success_filler == spi.m_filler);
    expect(that % expected_payload.data() == spi.m_out.data());
    expect(that % expected_payload.size() == spi.m_out.size());
    expect(std::equal(
      expected_buffer.begin(), expected_buffer.end(), actual_array.begin()));
  };

  "[failure] write_then_read<Length>"_test = []() {
    // Setup
    dummy spi;
    const std::array<hal::byte, 4> expected_payload{};

    // Exercise
    auto result = write_then_read<5>(spi, expected_payload, failure_filler);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(failure_filler == spi.m_filler);
    expect(that % expected_payload.data() == spi.m_out.data());
    expect(that % expected_payload.size() == spi.m_out.size());
  };
};
}  // namespace hal
