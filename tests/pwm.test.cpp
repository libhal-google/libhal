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

#include <libhal/pwm.hpp>

#include <boost/ut.hpp>

namespace hal {
namespace {
constexpr auto expected_frequency = hertz(1'000);
constexpr auto expected_duty_cycle = float(0.5);

class test_pwm : public hal::pwm
{
public:
  hertz m_frequency{};
  float m_duty_cycle{};
  bool m_return_error_status{ false };

  ~test_pwm() override = default;

private:
  result<frequency_t> driver_frequency(hertz p_frequency) override
  {
    m_frequency = p_frequency;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return frequency_t{};
  };
  result<duty_cycle_t> driver_duty_cycle(float p_duty_cycle) override
  {
    m_duty_cycle = p_duty_cycle;
    if (m_return_error_status) {
      return hal::new_error();
    }
    return duty_cycle_t{};
  };
};
}  // namespace

void pwm_test()
{
  using namespace boost::ut;
  "pwm interface test"_test = []() {
    // Setup
    test_pwm test;

    // Exercise
    auto result1 = test.frequency(expected_frequency);
    auto result2 = test.duty_cycle(expected_duty_cycle);

    // Verify
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(that % expected_frequency == test.m_frequency);
    expect(that % expected_duty_cycle == test.m_duty_cycle);
  };

  "pwm errors test"_test = []() {
    // Setup
    test_pwm test;
    test.m_return_error_status = true;

    // Exercise
    auto result1 = test.frequency(expected_frequency);
    auto result2 = test.duty_cycle(expected_duty_cycle);

    // Verify
    expect(!bool{ result1 });
    expect(!bool{ result2 });
  };
};
}  // namespace hal
