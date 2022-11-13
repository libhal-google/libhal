#include <libhal/serial/coroutines.hpp>

#include <boost/ut.hpp>
#include <libhal/testing.hpp>
#include <span>

namespace hal {
boost::ut::suite serial_skip_past_test = []() {
  using namespace boost::ut;

  class fake_serial : public hal::serial
  {
  public:
    status driver_configure(const settings&) noexcept override
    {
      return {};
    }

    result<write_t> driver_write(
      std::span<const hal::byte> p_data) noexcept override
    {
      return write_t{
        .transmitted = p_data,
        .remaining = std::span<const hal::byte>(),
      };
    }

    result<read_t> driver_read(std::span<hal::byte> p_data) noexcept override
    {
      return m_read_function(p_data);
    }

    status driver_flush() noexcept override
    {
      return {};
    }

    virtual ~fake_serial()
    {}

    std::function<result<read_t>(std::span<hal::byte>)> m_read_function;
  };

  const std::array<hal::byte, 6> read_out{ 1, 2, 3, 4, 5, 6 };
  const std::array<hal::byte, 3> sequence{ 3, 4, 5 };

  "[success] skip_past one shot"_test = [=]() {
    // Setup
    fake_serial serial;
    serial.m_read_function =
      [&read_out, count = size_t(0)](
        std::span<hal::byte> p_data) mutable -> result<serial::read_t> {
      if (count < read_out.size()) {
        p_data[0] = read_out[count++];

        return serial::read_t{
          .received = p_data.subspan(0, 1),
          .remaining = p_data.subspan(1),
          .available = 1,
          .capacity = 1,
        };
      }

      return serial::read_t{
        .received = p_data.subspan(0, 0),
        .remaining = std::span<hal::byte>(),
        .available = 1,
        .capacity = 1,
      };
    };

    auto skipper = skip_past(serial, sequence);
    std::array<work_state, 5> actual;

    // Exercise
    actual[0] = skipper().value();
    actual[1] = skipper().value();
    actual[2] = skipper().value();
    actual[3] = skipper().value();
    actual[4] = skipper().value();

    // Verify
    expect(that % work_state::finished == actual[0]);
    expect(that % work_state::finished == actual[1]);
    expect(that % work_state::finished == actual[2]);
    expect(that % work_state::finished == actual[3]);
    expect(that % work_state::finished == actual[4]);
  };

  "[success] skip_past one byte at a time"_test = [=]() {
    // Setup
    fake_serial serial;
    serial.m_read_function =
      [&read_out, count = size_t(0), send_byte = false](
        std::span<hal::byte> p_data) mutable -> result<serial::read_t> {
      send_byte = !send_byte;

      if (send_byte) {
        if (count < read_out.size()) {
          p_data[0] = read_out[count++];

          return serial::read_t{
            .received = p_data.subspan(0, 1),
            .remaining = p_data.subspan(1),
            .available = 1,
            .capacity = 1,
          };
        }
      }
      return serial::read_t{
        .received = p_data.subspan(0, 0),
        .remaining = std::span<hal::byte>(),
        .available = 1,
        .capacity = 1,
      };
    };

    auto skipper = skip_past(serial, sequence);
    std::array<work_state, 5> actual;

    // Exercise
    actual[0] = skipper().value();
    actual[1] = skipper().value();
    actual[2] = skipper().value();
    actual[3] = skipper().value();
    actual[4] = skipper().value();

    // Verify
    expect(that % work_state::in_progress == actual[0]);
    expect(that % work_state::in_progress == actual[1]);
    expect(that % work_state::in_progress == actual[2]);
    expect(that % work_state::in_progress == actual[3]);
    expect(that % work_state::finished == actual[4]);
  };

  "[success] skip_past in two blocks"_test = [=]() {
    // Setup
    fake_serial serial;
    serial.m_read_function =
      [&read_out, count = size_t(0)](
        std::span<hal::byte> p_data) mutable -> result<serial::read_t> {
      serial::read_t read_response{};

      switch (count) {
        case 0:
        case 1:
          p_data[0] = read_out[count];
          read_response = serial::read_t{
            .received = p_data.subspan(0, 1),
            .remaining = p_data.subspan(1),
            .available = 1,
            .capacity = 1,
          };
          break;
        case 2:
          read_response = serial::read_t{
            .received = p_data.subspan(0, 0),
            .remaining = std::span<hal::byte>(),
            .available = 1,
            .capacity = 1,
          };
          break;
        case 3:
          p_data[0] = read_out[count - 1];
          read_response = serial::read_t{
            .received = p_data.subspan(0, 1),
            .remaining = p_data.subspan(1),
            .available = 1,
            .capacity = 1,
          };
          break;
        case 4:
          read_response = serial::read_t{
            .received = p_data.subspan(0, 0),
            .remaining = std::span<hal::byte>(),
            .available = 1,
            .capacity = 1,
          };
          break;
        case 5:
        case 6:
        case 7:
          p_data[0] = read_out[count - 2];
          read_response = serial::read_t{
            .received = p_data.subspan(0, 1),
            .remaining = p_data.subspan(1),
            .available = 1,
            .capacity = 1,
          };
          break;
        default:
          read_response = serial::read_t{
            .received = p_data.subspan(0, 0),
            .remaining = std::span<hal::byte>(),
            .available = 1,
            .capacity = 1,
          };
      }
      count++;
      return read_response;
    };

    auto skipper = skip_past(serial, sequence);
    std::array<work_state, 5> actual;

    // Exercise
    actual[0] = skipper().value();
    actual[1] = skipper().value();
    actual[2] = skipper().value();
    actual[3] = skipper().value();
    actual[4] = skipper().value();

    // Verify
    expect(that % work_state::in_progress == actual[0]);
    expect(that % work_state::in_progress == actual[1]);
    expect(that % work_state::finished == actual[2]);
    expect(that % work_state::finished == actual[3]);
    expect(that % work_state::finished == actual[4]);
  };

  "[success] skip_past return control after 1 byte"_test = [=]() {
    // Setup
    fake_serial serial;
    serial.m_read_function =
      [&read_out, count = size_t(0)](
        std::span<hal::byte> p_data) mutable -> result<serial::read_t> {
      if (count < read_out.size()) {
        p_data[0] = read_out[count++];

        return serial::read_t{
          .received = p_data.subspan(0, 1),
          .remaining = p_data.subspan(1),
          .available = 1,
          .capacity = 1,
        };
      }

      return serial::read_t{
        .received = p_data.subspan(0, 0),
        .remaining = std::span<hal::byte>(),
        .available = 1,
        .capacity = 1,
      };
    };

    auto skipper = skip_past(serial, sequence, 1);
    std::array<work_state, 6> actual;

    // Exercise
    actual[0] = skipper().value();
    actual[1] = skipper().value();
    actual[2] = skipper().value();
    actual[3] = skipper().value();
    actual[4] = skipper().value();
    actual[5] = skipper().value();

    // Verify
    expect(that % work_state::in_progress == actual[0]);
    expect(that % work_state::in_progress == actual[1]);
    expect(that % work_state::in_progress == actual[2]);
    expect(that % work_state::in_progress == actual[3]);
    expect(that % work_state::finished == actual[4]);
    expect(that % work_state::finished == actual[5]);
  };

  "[failure] skip_past return error on read"_test = [=]() {
    // Setup
    fake_serial serial;
    serial.m_read_function =
      []([[maybe_unused]] std::span<hal::byte> p_data) mutable
      -> result<serial::read_t> { return hal::new_error(); };

    auto skipper = skip_past(serial, sequence);

    // Exercise
    auto actual = skipper();

    // Verify
    expect(that % false == bool(actual));
  };

  "[success] skip_past error every other byte"_test = [=]() {
    // Setup
    fake_serial serial;
    serial.m_read_function =
      [&read_out, count = size_t(0), send_byte = false](
        std::span<hal::byte> p_data) mutable -> result<serial::read_t> {
      send_byte = !send_byte;

      if (send_byte) {
        if (count < read_out.size()) {
          p_data[0] = read_out[count++];

          return serial::read_t{
            .received = p_data.subspan(0, 1),
            .remaining = p_data.subspan(1),
            .available = 1,
            .capacity = 1,
          };
        }
      }

      return hal::new_error();
    };

    auto skipper = skip_past(serial, sequence, 1);

    // Exercise
    auto actual0 = skipper();
    auto actual1 = skipper();
    auto actual2 = skipper();
    auto actual3 = skipper();
    auto actual4 = skipper();
    auto actual5 = skipper();
    auto actual6 = skipper();
    auto actual7 = skipper();
    auto actual8 = skipper();
    auto actual9 = skipper();
    auto actual10 = skipper();

    // Verify
    expect(that % true == actual0.has_value());
    expect(that % false == actual1.has_value());
    expect(that % true == actual2.has_value());
    expect(that % false == actual3.has_value());
    expect(that % true == actual4.has_value());
    expect(that % false == actual5.has_value());
    expect(that % true == actual6.has_value());
    expect(that % false == actual7.has_value());
    expect(that % true == actual8.has_value());  // finished
    expect(that % true == actual9.has_value());  // can no longer error out
                                                 // because this is finished.
    expect(that % true == actual10.has_value());

    expect(that % work_state::in_progress == actual0.value());
    expect(that % work_state::in_progress == actual2.value());
    expect(that % work_state::in_progress == actual4.value());
    expect(that % work_state::in_progress == actual6.value());
    expect(that % work_state::finished == actual8.value());
    expect(that % work_state::finished == actual10.value());
  };
};

boost::ut::suite serial_read_into_test = []() {
  using namespace boost::ut;

  class fake_serial : public hal::serial
  {
  public:
    status driver_configure(const settings&) noexcept override
    {
      return {};
    }

    result<write_t> driver_write(
      std::span<const hal::byte> p_data) noexcept override
    {
      return write_t{
        .transmitted = p_data,
        .remaining = std::span<const hal::byte>(),
      };
    }

    result<read_t> driver_read(std::span<hal::byte> p_data) noexcept override
    {
      return m_read_function(p_data);
    }

    status driver_flush() noexcept override
    {
      return {};
    }

    virtual ~fake_serial()
    {}

    std::function<result<read_t>(std::span<hal::byte>)> m_read_function;
  };

  "[success] read_into one shot"_test = [=]() {
    // Setup
    fake_serial serial;
    serial.m_read_function =
      [](std::span<hal::byte> p_data) mutable -> result<serial::read_t> {
      size_t counter = 0;

      for (auto& data : p_data) {
        data = static_cast<hal::byte>(counter++);
      }

      return serial::read_t{
        .received = p_data,
        .remaining = p_data.subspan(p_data.size()),
        .available = 0,
        .capacity = 1024,
      };
    };

    const std::array<hal::byte, 4> expected = { 0, 1, 2, 3 };
    std::array<hal::byte, 4> actual_buffer;

    auto reader = read_into(serial, actual_buffer);
    std::array<work_state, 5> results;

    // Exercise
    results[0] = reader().value();
    results[1] = reader().value();
    results[2] = reader().value();
    results[3] = reader().value();
    results[4] = reader().value();

    // Verify
    expect(that % work_state::finished == results[0]);
    expect(that % work_state::finished == results[1]);
    expect(that % work_state::finished == results[2]);
    expect(that % work_state::finished == results[3]);
    expect(that % work_state::finished == results[4]);
    expect(that % actual_buffer == expected);
  };

  "[success] read_into one byte at a time"_test = [=]() {
    // Setup
    fake_serial serial;
    serial.m_read_function =
      [counter = size_t(0)](
        std::span<hal::byte> p_data) mutable -> result<serial::read_t> {
      p_data[0] = static_cast<hal::byte>(counter++);

      return serial::read_t{
        .received = p_data.subspan(0, 1),
        .remaining = p_data.subspan(1),
        .available = 1,
        .capacity = 1024,
      };
    };

    const std::array<hal::byte, 4> expected = { 0, 1, 2, 3 };
    std::array<hal::byte, 4> actual_buffer;

    auto reader = read_into(serial, actual_buffer);
    std::array<work_state, 5> results;

    // Exercise
    results[0] = reader().value();
    results[1] = reader().value();
    results[2] = reader().value();
    results[3] = reader().value();
    results[4] = reader().value();

    // Verify
    expect(that % work_state::finished == results[0]);
    expect(that % work_state::finished == results[1]);
    expect(that % work_state::finished == results[2]);
    expect(that % work_state::finished == results[3]);
    expect(that % work_state::finished == results[4]);
    expect(that % actual_buffer == expected);
  };

  "[success] read_into return control after 1 byte"_test = [=]() {
    // Setup
    fake_serial serial;
    serial.m_read_function =
      [counter = size_t(0)](
        std::span<hal::byte> p_data) mutable -> result<serial::read_t> {
      p_data[0] = static_cast<hal::byte>(counter++);

      return serial::read_t{
        .received = p_data.subspan(0, 1),
        .remaining = p_data.subspan(1),
        .available = 1,
        .capacity = 1024,
      };
    };

    const std::array<hal::byte, 4> expected = { 0, 1, 2, 3 };
    std::array<hal::byte, 4> actual_buffer;

    auto reader = read_into(serial, actual_buffer, 1);
    std::array<work_state, 5> results;

    // Exercise
    results[0] = reader().value();
    results[1] = reader().value();
    results[2] = reader().value();
    results[3] = reader().value();
    results[4] = reader().value();

    // Verify
    expect(that % work_state::in_progress == results[0]);
    expect(that % work_state::in_progress == results[1]);
    expect(that % work_state::in_progress == results[2]);
    expect(that % work_state::in_progress == results[3]);
    expect(that % work_state::finished == results[4]);
    expect(that % actual_buffer == expected);
  };

  "[failure] read_into return error on read"_test = [=]() {
    // Setup
    fake_serial serial;
    serial.m_read_function =
      []([[maybe_unused]] std::span<hal::byte> p_data) mutable
      -> result<serial::read_t> { return hal::new_error(); };

    const std::array<hal::byte, 2> expected = { 0, 1 };
    std::array<hal::byte, 2> actual_buffer;

    auto reader = read_into(serial, actual_buffer);

    // Exercise
    auto result = reader();

    // Verify
    expect(that % result.has_error());
    expect(that % actual_buffer != expected);
  };
};

boost::ut::suite serial_read_upto_test = []() {
  using namespace boost::ut;

  class fake_serial : public hal::serial
  {
  public:
    status driver_configure(const settings&) noexcept override
    {
      return {};
    }

    result<write_t> driver_write(
      std::span<const hal::byte> p_data) noexcept override
    {
      return write_t{
        .transmitted = p_data,
        .remaining = std::span<const hal::byte>(),
      };
    }

    result<read_t> driver_read(std::span<hal::byte> p_data) noexcept override
    {
      return m_read_function(p_data);
    }

    status driver_flush() noexcept override
    {
      return {};
    }

    virtual ~fake_serial()
    {}

    std::function<result<read_t>(std::span<hal::byte>)> m_read_function;
  };

  "[success] read_upto one shot"_test = [=]() {
    // Setup
    fake_serial serial;
    serial.m_read_function =
      [counter = size_t(0)](
        std::span<hal::byte> p_data) mutable -> result<serial::read_t> {
      for (auto& data : p_data) {
        data = static_cast<hal::byte>(counter++);
      }

      return serial::read_t{
        .received = p_data,
        .remaining = p_data.subspan(p_data.size()),
        .available = 0,
        .capacity = 1024,
      };
    };

    const std::array<hal::byte, 4> expected = { 0, 1, 2, 3 };
    const std::array<hal::byte, 2> sequence = { 2, 3 };
    std::array<hal::byte, 4> actual_buffer{};

    auto reader = read_upto(serial, sequence, actual_buffer);
    std::array<work_state, 5> results;

    // Exercise
    results[0] = reader().value();
    results[1] = reader().value();
    results[2] = reader().value();
    results[3] = reader().value();
    results[4] = reader().value();

    // Verify
    expect(that % work_state::finished == results[0]);
    expect(that % work_state::finished == results[1]);
    expect(that % work_state::finished == results[2]);
    expect(that % work_state::finished == results[3]);
    expect(that % work_state::finished == results[4]);
    expect(that % actual_buffer == expected);
  };

  "[success] read_upto one byte at a time"_test = [=]() {
    // Setup
    fake_serial serial;
    serial.m_read_function =
      [counter = size_t(0)](
        std::span<hal::byte> p_data) mutable -> result<serial::read_t> {
      for (auto& data : p_data) {
        data = static_cast<hal::byte>(counter++);
      }

      return serial::read_t{
        .received = p_data,
        .remaining = p_data.subspan(p_data.size()),
        .available = 0,
        .capacity = 1024,
      };
    };

    const std::array<hal::byte, 4> expected = { 0, 1, 2, 3 };
    const std::array<hal::byte, 2> sequence = { 2, 3 };
    std::array<hal::byte, 4> actual_buffer{};

    auto reader = read_upto(serial, sequence, actual_buffer, 1);
    std::array<work_state, 5> results;

    // Exercise
    results[0] = reader().value();
    results[1] = reader().value();
    results[2] = reader().value();
    results[3] = reader().value();
    results[4] = reader().value();

    // Verify
    expect(that % work_state::in_progress == results[0]);
    expect(that % work_state::in_progress == results[1]);
    expect(that % work_state::in_progress == results[2]);
    expect(that % work_state::finished == results[3]);
    expect(that % work_state::finished == results[4]);
    expect(that % actual_buffer == expected);
  };

  "[success] read_upto in two blocks"_test = [=]() {};
  "[failure] read_upto return error on read"_test = [=]() {
    // Setup
    fake_serial serial;
    serial.m_read_function =
      []([[maybe_unused]] std::span<hal::byte> p_data) mutable
      -> result<serial::read_t> { return hal::new_error(); };

    const std::array<hal::byte, 4> expected = { 0, 1, 2, 3 };
    const std::array<hal::byte, 2> sequence = { 2, 3 };
    std::array<hal::byte, 4> actual_buffer;

    auto reader = read_upto(serial, sequence, actual_buffer);

    // Exercise
    auto result = reader();

    // Verify
    expect(that % result.has_error());
    expect(that % actual_buffer != expected);
  };
};
}  // namespace hal