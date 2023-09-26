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

#include <libhal/output_pin.hpp>

#include <boost/ut.hpp>

namespace hal {
namespace {
constexpr hal::output_pin::settings expected_settings{
  .resistor = pin_resistor::pull_down,
  .open_drain = true,
};
class test_output_pin : public hal::output_pin
{
public:
  settings m_settings{};
  bool m_driver_level{};
  bool m_return_error_status{ false };

  ~test_output_pin() override = default;

private:
  status driver_configure(const settings& p_settings) override
  {
    m_settings = p_settings;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return success();
  };
  result<set_level_t> driver_level(bool p_high) override
  {
    m_driver_level = p_high;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return set_level_t{};
  };
  result<level_t> driver_level() override
  {
    if (m_return_error_status) {
      return hal::new_error();
    }
    return level_t{ .state = m_driver_level };
  };
};
}  // namespace

void output_pin_test()
{
  using namespace boost::ut;
  "output_pin interface test"_test = []() {
    // Setup
    test_output_pin test;

    // Exercise
    auto result1 = test.configure(expected_settings);
    auto result2 = test.level(true);
    auto result3 = test.level();

    // Verify
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(bool{ result3 });
    expect(expected_settings.open_drain == test.m_settings.open_drain);
    expect(expected_settings.resistor == test.m_settings.resistor);
    expect(that % true == test.m_driver_level);
    expect(that % true == result3.value().state);
  };

  "output_pin errors test"_test = []() {
    // Setup
    test_output_pin test;
    test.m_return_error_status = true;

    // Exercise
    auto result1 = test.configure(expected_settings);
    auto result2 = test.level(true);
    auto result3 = test.level();

    // Verify
    expect(!bool{ result1 });
    expect(!bool{ result2 });
    expect(!bool{ result3 });
  };
};
}  // namespace hal
