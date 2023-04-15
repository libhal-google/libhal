// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <libhal/socket.hpp>

#include <boost/ut.hpp>

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
    hal::function_ref<hal::timeout_function> p_timeout)
  {
    if (m_return_error_status) {
      return new_error();
    }
    m_write = p_data;
    (void)p_timeout();
    return write_t{ p_data.first(2) };
  }
  hal::result<read_t> driver_read(std::span<hal::byte> p_data)
  {
    if (m_return_error_status) {
      return new_error();
    }
    m_read = p_data;
    return read_t{ p_data.first(2) };
  }
};
}  // namespace

void socket_test()
{
  using namespace boost::ut;
  hal::function_ref<timeout_function> always_succeed = []() -> hal::status {
    return hal::success();
  };

  "socket::write() && socket::read() success"_test = [always_succeed]() {
    // Setup
    test_socket test;
    std::array<hal::byte, 4> buffer;

    // Exercise
    auto write_result = test.write(buffer, always_succeed);
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
  };

  "socket::write() && socket::read() failure"_test = [always_succeed]() {
    // Setup
    test_socket test;
    test.m_return_error_status = true;
    std::array<hal::byte, 4> buffer;

    // Exercise
    auto write_result = test.write(buffer, always_succeed);
    auto read_result = test.read(buffer);

    // Verify
    expect(!bool{ write_result });
    expect(!bool{ read_result });
  };
};
}  // namespace hal
