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

#include <optional>
#include <utility>

#include <libhal/error.hpp>
#include <libhal/units.hpp>

#include <boost/ut.hpp>

namespace hal {
struct uuid_t
{
  std::array<hal::byte, 16> uuid{};
};

consteval uuid_t to_uuid([[maybe_unused]] const std::string_view p_str)
{
  return uuid_t{};
}

struct error_metadata_t
{
  // 2 bits of info
  // Maybe these should be different types of error_t rather than metadata
  enum class classification : uint8_t
  {
    transient,
    recoverable,
    fatal,
  };

  enum class recovery_hint : uint8_t
  {
    retry,
    reset_hardware,
    change_configuration,
  };

  enum class source : uint8_t
  {
    function,
    peripheral,
    device,
    soft,
    application,
  };

  std::uint64_t raw{};
};

struct message_id_t
{
  std::uint16_t raw{};
  std::array<hal::byte, 16> additional_data{};
};

struct header_t
{
  std::array<char, 8> header{ 'L', 'I', 'B', 'H', 'A', 'L', 'E', 'X' };
};

struct error_t
{
  header_t header{};
  uuid_t uuid{};
  void* driver_instance{};
  error_metadata_t metadata{};
  std::errc error_code{};
  std::optional<message_id_t> message_id{};

  static constexpr size_t uuid_size = sizeof(uuid);
  static constexpr size_t metadata_size = sizeof(metadata);
  static constexpr size_t error_code_size = sizeof(error_code);
  static constexpr size_t message_id_size = sizeof(message_id);
};

[[maybe_unused]] constexpr size_t error_size = sizeof(error_t);

void error_test()
{
  using namespace boost::ut;

  "[success] hal::safe_throw"_test = []() {
    // Setup
    bool exception_thrown = false;

    try {
      hal::safe_throw(5);
    } catch (int) {
      exception_thrown = true;
    }
    expect(exception_thrown);
  };

  "[success] hal::safe_throw"_test = []() {
    // Setup
    bool exception_thrown = false;

    struct pod_t
    {
      float value;
    };

    try {
      hal::safe_throw(pod_t{ .value = 5.0f });
    } catch (pod_t) {
      exception_thrown = true;
    }

    expect(exception_thrown);
  };

#define TEST_COMPILE_TIME_FAILURE 0
#if TEST_COMPILE_TIME_FAILURE
  "[success] hal::safe_throw(non-trivial-dtor)"_test = []() {
    // Setup
    bool exception_thrown = false;

    struct dtor_t
    {
      float value;
      ~dtor_t()
      {
        value = 0;
      }
    };

    try {
      hal::safe_throw(dtor_t{ .value = 5.0f });
    } catch (dtor_t) {
      exception_thrown = true;
    }

    expect(exception_thrown);
  };
#endif
};
}  // namespace hal
