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

#include <libhal/error.hpp>
#include <libhal/serial.hpp>

#include <boost/ut.hpp>

namespace hal {
namespace {
constexpr hal::serial::settings expected_settings{
  .baud_rate = 115200.0f,
  .stop = hal::serial::settings::stop_bits::two,
  .parity = hal::serial::settings::parity::none
};

class test_serial : public hal::serial
{
public:
  settings m_settings{};
  bool m_flush_called{ false };
  ~test_serial() override = default;

private:
  void driver_configure(const settings& p_settings) override
  {
    m_settings = p_settings;
  }

  write_t driver_write(std::span<const hal::byte> p_data) override
  {
    return write_t{ p_data };
  }

  read_t driver_read(std::span<hal::byte> p_data) override
  {
    return read_t{
      .data = p_data.subspan(0, 1),
      .available = 1,
      .capacity = 1,
    };
  }

  flush_t driver_flush() override
  {
    m_flush_called = true;
    return flush_t{};
  }
};
}  // namespace

void serial_test()
{
  using namespace boost::ut;
  "serial interface test"_test = []() {
    // Setup
    test_serial test;
    const std::array<hal::byte, 4> expected_payload{ 'a', 'b' };
    std::array<hal::byte, 4> expected_buffer{ '1', '2' };

    // Exercise
    test.configure(expected_settings);
    auto write_info = test.write(expected_payload);
    auto read_info = test.read(expected_buffer);
    test.flush();

    // Verify
    auto delta = expected_settings.baud_rate - test.m_settings.baud_rate;
    expect(that % 0.001f > std::abs(delta));
    expect(expected_settings.stop == test.m_settings.stop);
    expect(expected_settings.parity == test.m_settings.parity);
    expect(that % expected_payload.data() == write_info.data.data());
    expect(that % expected_buffer.data() == read_info.data.data());
    expect(true == test.m_flush_called);
  };
};
}  // namespace hal
