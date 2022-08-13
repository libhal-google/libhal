#include <boost/ut.hpp>
#include <libhal/i2c/util.hpp>
#include <span>

namespace hal {
boost::ut::suite i2c_util_test = []() {
  using namespace boost::ut;

  static constexpr hal::byte successful_address{ 0x15 };
  static constexpr hal::byte failure_address{ 0x33 };
  static constexpr hal::byte filler_byte{ 0xA5 };

  struct test_timeout_t
  {
    status operator()()
    {
      was_called = true;
      return {};
    }
    bool was_called = false;
  };

  class test_i2c : public hal::i2c
  {
  public:
    [[nodiscard]] status driver_configure(const settings&) noexcept override
    {
      return {};
    }
    [[nodiscard]] status driver_transaction(
      hal::byte p_address,
      std::span<const hal::byte> p_out,
      std::span<hal::byte> p_in,
      std::function<hal::timeout> p_timeout) noexcept override
    {
      m_address = p_address;
      m_out = p_out;
      m_in = p_in;

      std::fill(m_in.begin(), m_in.end(), filler_byte);

      if (m_address == failure_address) {
        return hal::new_error();
      }

      (void)p_timeout();

      return {};
    }

    virtual ~test_i2c()
    {
    }

    hal::byte m_address = hal::byte{ 0 };
    std::span<const hal::byte> m_out = std::span<const hal::byte>{};
    std::span<hal::byte> m_in = std::span<hal::byte>{};
  };

  "[success] write"_test = []() {
    // Setup
    test_i2c i2c;
    test_timeout_t test_timeout;
    const std::array<hal::byte, 4> expected_payload{};

    // Exercise
    auto result =
      write(i2c, successful_address, expected_payload, std::ref(test_timeout));
    bool successful = static_cast<bool>(result);

    // Verify
    expect(successful);
    expect(successful_address == i2c.m_address);
    expect(that % expected_payload.data() == i2c.m_out.data());
    expect(that % expected_payload.size() == i2c.m_out.size());
    expect(that % nullptr == i2c.m_in.data());
    expect(that % 0 == i2c.m_in.size());
    expect(that % true == test_timeout.was_called);
  };

  "[failure] write"_test = []() {
    // Setup
    test_i2c i2c;
    test_timeout_t test_timeout;
    const std::array<hal::byte, 4> expected_payload{};

    // Exercise
    auto result =
      write(i2c, failure_address, expected_payload, std::ref(test_timeout));
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(failure_address == i2c.m_address);
    expect(that % expected_payload.data() == i2c.m_out.data());
    expect(that % expected_payload.size() == i2c.m_out.size());
    expect(that % nullptr == i2c.m_in.data());
    expect(that % 0 == i2c.m_in.size());
    expect(that % false == test_timeout.was_called);
  };

  "[success] read"_test = []() {
    // Setup
    test_i2c i2c;
    test_timeout_t test_timeout;
    std::array<hal::byte, 4> expected_buffer;

    // Exercise
    auto result =
      read(i2c, successful_address, expected_buffer, std::ref(test_timeout));
    bool successful = static_cast<bool>(result);

    // Verify
    expect(successful);
    expect(successful_address == i2c.m_address);
    expect(that % expected_buffer.data() == i2c.m_in.data());
    expect(that % expected_buffer.size() == i2c.m_in.size());
    expect(that % nullptr == i2c.m_out.data());
    expect(that % 0 == i2c.m_out.size());
    expect(that % true == test_timeout.was_called);
  };

  "[failure] read"_test = []() {
    // Setup
    test_i2c i2c;
    test_timeout_t test_timeout;
    std::array<hal::byte, 4> expected_buffer;

    // Exercise
    auto result =
      read(i2c, failure_address, expected_buffer, std::ref(test_timeout));
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(failure_address == i2c.m_address);
    expect(that % expected_buffer.data() == i2c.m_in.data());
    expect(that % expected_buffer.size() == i2c.m_in.size());
    expect(that % nullptr == i2c.m_out.data());
    expect(that % 0 == i2c.m_out.size());
    expect(that % false == test_timeout.was_called);
  };

  "[success] read<Length>"_test = []() {
    // Setup
    test_i2c i2c;
    test_timeout_t test_timeout;
    std::array<hal::byte, 5> expected_buffer;
    expected_buffer.fill(filler_byte);

    // Exercise
    auto result = read<expected_buffer.size()>(
      i2c, successful_address, std::ref(test_timeout));
    bool successful = static_cast<bool>(result);

    // Verify
    expect(successful);
    expect(successful_address == i2c.m_address);
    expect(std::equal(
      expected_buffer.begin(), expected_buffer.end(), result.value().begin()));
    expect(that % nullptr == i2c.m_out.data());
    expect(that % 0 == i2c.m_out.size());
    expect(that % true == test_timeout.was_called);
  };

  "[failure] read<Length>"_test = []() {
    // Setup
    test_i2c i2c;
    test_timeout_t test_timeout;

    // Exercise
    auto result = read<5>(i2c, failure_address);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(failure_address == i2c.m_address);
    expect(that % nullptr == i2c.m_out.data());
    expect(that % 0 == i2c.m_out.size());
    expect(that % false == test_timeout.was_called);
  };

  "[success] write_then_read"_test = []() {
    // Setup
    test_i2c i2c;
    test_timeout_t test_timeout;
    const std::array<hal::byte, 4> expected_payload{};
    std::array<hal::byte, 4> expected_buffer;

    // Exercise
    auto result = write_then_read(i2c,
                                  successful_address,
                                  expected_payload,
                                  expected_buffer,
                                  std::ref(test_timeout));
    bool successful = static_cast<bool>(result);

    // Verify
    expect(successful);
    expect(successful_address == i2c.m_address);
    expect(that % expected_payload.data() == i2c.m_out.data());
    expect(that % expected_payload.size() == i2c.m_out.size());
    expect(that % expected_buffer.data() == i2c.m_in.data());
    expect(that % expected_buffer.size() == i2c.m_in.size());
    expect(that % true == test_timeout.was_called);
  };

  "[failure] write_then_read"_test = []() {
    // Setup
    test_i2c i2c;
    test_timeout_t test_timeout;
    const std::array<hal::byte, 4> expected_payload{};
    std::array<hal::byte, 4> expected_buffer;
    expected_buffer.fill(filler_byte);

    // Exercise
    auto result = write_then_read(i2c,
                                  failure_address,
                                  expected_payload,
                                  expected_buffer,
                                  std::ref(test_timeout));
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(failure_address == i2c.m_address);
    expect(that % expected_payload.data() == i2c.m_out.data());
    expect(that % expected_payload.size() == i2c.m_out.size());
    expect(that % expected_buffer.data() == i2c.m_in.data());
    expect(that % expected_buffer.size() == i2c.m_in.size());
    expect(that % false == test_timeout.was_called);
  };

  "[success] write_then_read<Length>"_test = []() {
    // Setup
    test_i2c i2c;
    test_timeout_t test_timeout;
    const std::array<hal::byte, 4> expected_payload{};
    std::array<hal::byte, 4> expected_buffer{};
    expected_buffer.fill(filler_byte);

    // Exercise
    auto result = write_then_read<5>(
      i2c, successful_address, expected_payload, std::ref(test_timeout));
    bool successful = static_cast<bool>(result);
    auto actual_array = result.value();

    // Verify
    expect(successful);
    expect(successful_address == i2c.m_address);
    expect(that % expected_payload.data() == i2c.m_out.data());
    expect(that % expected_payload.size() == i2c.m_out.size());
    expect(std::equal(
      expected_buffer.begin(), expected_buffer.end(), actual_array.begin()));
    expect(that % true == test_timeout.was_called);
  };

  "[failure] write_then_read<Length>"_test = []() {
    // Setup
    test_i2c i2c;
    test_timeout_t test_timeout;
    const std::array<hal::byte, 4> expected_payload{};

    // Exercise
    auto result = write_then_read<5>(
      i2c, failure_address, expected_payload, std::ref(test_timeout));
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(failure_address == i2c.m_address);
    expect(that % expected_payload.data() == i2c.m_out.data());
    expect(that % expected_payload.size() == i2c.m_out.size());
    expect(that % false == test_timeout.was_called);
  };

  "[success] probe(i2c&)"_test = []() {
    // Setup
    test_i2c i2c;

    // Exercise
    auto result = probe(i2c, successful_address);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(successful);
    expect(successful_address == i2c.m_address);
    expect(that % 1 == i2c.m_in.size());
    expect(that % nullptr != i2c.m_in.data());
    expect(that % 0 == i2c.m_out.size());
    expect(that % nullptr == i2c.m_out.data());
  };

  "[failure] probe(i2c&)"_test = []() {
    // Setup
    test_i2c i2c;

    // Exercise
    auto result = probe(i2c, failure_address);
    bool successful = static_cast<bool>(result);

    // Verify
    expect(!successful);
    expect(failure_address == i2c.m_address);
    expect(that % 1 == i2c.m_in.size());
    expect(that % nullptr != i2c.m_in.data());
    expect(that % 0 == i2c.m_out.size());
    expect(that % nullptr == i2c.m_out.data());
  };
};
}  // namespace hal
