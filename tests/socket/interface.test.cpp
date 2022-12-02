#include <libhal/socket/interface.hpp>

#include <boost/ut.hpp>
#include <libhal/callback_recorder.hpp>
#include <libhal/testing.hpp>

namespace hal {
namespace {

class test_socket : public hal::socket
{
public:
  std::span<const hal::byte> m_write;
  std::span<hal::byte> m_read;
  bool m_return_error_status = false;

  ~test_socket()
  {
  }

private:
  hal::result<write_t> driver_write(
    std::span<const hal::byte> p_data,
    std::function<hal::timeout_function> p_timeout) noexcept
  {
    if (m_return_error_status) {
      return new_error();
    }
    m_write = p_data;
    (void)p_timeout();
    return write_t{ p_data.first(2) };
  }
  hal::result<read_t> driver_read(std::span<hal::byte> p_data) noexcept
  {
    if (m_return_error_status) {
      return new_error();
    }
    m_read = p_data;
    return read_t{ p_data.first(2) };
  }
};
}  // namespace

boost::ut::suite socket_test = []() {
  using namespace boost::ut;
  "socket interface test"_test = []() {
    // Setup
    test_socket test;
    std::array<hal::byte, 4> buffer;
    std::function<timeout_function> always_succeed = []() -> hal::status {
      return hal::success();
    };
    hal::mock::callback_recorder<timeout_function> recorder(always_succeed);

    // Exercise
    auto write_result = test.write(buffer, recorder.callback());
    auto read_result = test.read(buffer);

    // Verify
    expect(bool{ write_result });
    expect(bool{ read_result });
    expect(that % buffer.data() == test.m_write.data());
    expect(that % buffer.size() == test.m_write.size());
    expect(that % buffer.data() == test.m_read.data());
    expect(that % buffer.size() == test.m_read.size());
    expect(that % write_result.value().data.data() == test.m_write.data());
    expect(that % write_result.value().data.size() == 2);
    expect(that % read_result.value().data.data() == test.m_read.data());
    expect(that % read_result.value().data.size() == 2);
    expect(that % true == recorder.was_called_once());
  };

  "socket errors test"_test = []() {
    // Setup
    test_socket test;
    test.m_return_error_status = true;
    std::array<hal::byte, 4> buffer;
    std::function<timeout_function> always_succeed = []() -> hal::status {
      return hal::success();
    };
    hal::mock::callback_recorder<timeout_function> recorder(always_succeed);

    // Exercise
    auto write_result = test.write(buffer, recorder.callback());
    auto read_result = test.read(buffer);

    // Verify
    expect(!bool{ write_result });
    expect(!bool{ read_result });
    expect(that % false == recorder.was_called_once());
  };
};
}  // namespace hal
