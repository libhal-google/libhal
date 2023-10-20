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

  ~test_output_pin() override = default;

private:
  void driver_configure(const settings& p_settings) override
  {
    m_settings = p_settings;
  }
  set_level_t driver_level(bool p_high) override
  {
    m_driver_level = p_high;
    return set_level_t{};
  }
  level_t driver_level() override
  {
    return level_t{ .state = m_driver_level };
  }
};
}  // namespace

void output_pin_test()
{
  using namespace boost::ut;
  "output_pin interface test"_test = []() {
    // Setup
    test_output_pin test;

    // Exercise
    test.configure(expected_settings);
    test.level(true);
    auto level = test.level();

    // Verify
    expect(expected_settings.open_drain == test.m_settings.open_drain);
    expect(expected_settings.resistor == test.m_settings.resistor);
    expect(that % true == test.m_driver_level);
    expect(that % true == level.state);
  };
};
}  // namespace hal
