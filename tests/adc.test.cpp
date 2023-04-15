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

#include <libhal/adc.hpp>

#include <boost/ut.hpp>

namespace hal {
namespace {
constexpr auto expected_value = float(0.5);

class test_adc : public hal::adc
{
public:
  constexpr static float m_returned_position{ 0.5f };
  bool m_return_error_status{ false };

  ~test_adc()
  {
  }

private:
  result<read_t> driver_read() override
  {
    if (m_return_error_status) {
      return hal::new_error();
    }
    return read_t{ .sample = m_returned_position };
  }
};
}  // namespace

void adc_test()
{
  using namespace boost::ut;
  "adc interface test"_test = []() {
    // Setup
    test_adc test;

    // Exercise
    auto result = test.read();

    // Verify
    expect(bool{ result });
    expect(that % expected_value == result.value().sample);
  };

  "adc errors test"_test = []() {
    // Setup
    test_adc test;
    test.m_return_error_status = true;

    // Exercise
    auto result = test.read();

    // Verify
    expect(!bool{ result });
  };
}
}  // namespace hal
