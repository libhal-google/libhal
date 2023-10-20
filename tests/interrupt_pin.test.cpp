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
#include <libhal/interrupt_pin.hpp>

#include <functional>

#include <boost/ut.hpp>

namespace hal {
namespace {
constexpr hal::interrupt_pin::settings expected_settings{
  .resistor = pin_resistor::pull_down,
  .trigger = interrupt_pin::trigger_edge::rising,
};

class test_interrupt_pin : public hal::interrupt_pin
{
public:
  settings m_settings{};
  std::function<handler> m_callback = [](bool) {};

private:
  void driver_configure(const settings& p_settings) override
  {
    m_settings = p_settings;
  };
  void driver_on_trigger(hal::callback<handler> p_callback) override
  {
    m_callback = p_callback;
  };
};
}  // namespace

void interrupt_pin_test()
{
  using namespace boost::ut;
  "interrupt_pin interface test"_test = []() {
    // Setup
    test_interrupt_pin test;
    int counter = 0;
    auto expected_callback = [&counter](bool) { counter++; };

    // Exercise
    test.configure(expected_settings);
    test.on_trigger(expected_callback);
    test.m_callback(false);

    // Verify
    expect(expected_settings.resistor == test.m_settings.resistor);
    expect(expected_settings.trigger == test.m_settings.trigger);
    expect(that % 1 == counter);
  };
};
}  // namespace hal
