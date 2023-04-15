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

#include <libhal/servo.hpp>

#include <boost/ut.hpp>

namespace hal {
namespace {
constexpr auto expected_value = hal::degrees(45);
constexpr auto min_range = hal::degrees(-90);
constexpr auto max_range = hal::degrees(+90);

class test_servo : public hal::servo
{
public:
  hal::degrees m_position = 0.0f;

  ~test_servo()
  {
  }

private:
  result<position_t> driver_position(hal::degrees p_position) override
  {
    bool in_range = min_range < p_position && p_position < max_range;
    if (!in_range) {
      return hal::new_error(std::errc::invalid_argument,
                            hal::servo::range_error{
                              .min = min_range,
                              .max = max_range,
                            });
    }

    m_position = p_position;

    return position_t{};
  };
};
}  // namespace

void servo_test()
{
  using namespace boost::ut;
  "servo interface test"_test = []() {
    // Setup
    test_servo test;

    // Exercise
    auto result = test.position(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % expected_value == test.m_position);
  };

  "servo errors test"_test = []() {
    // Setup
    test_servo test;

    // Exercise
    hal::attempt_all(
      [&test]() -> hal::status {
        HAL_CHECK(test.position(max_range + 45.0f));
        return hal::new_error();
      },
      // Verify
      [](std::errc p_error_code, hal::servo::range_error p_range_error) {
        expect(std::errc::invalid_argument == p_error_code);
        expect(that % min_range == p_range_error.min);
        expect(that % max_range == p_range_error.max);
      },
      []() { expect(false) << "None of the above errors were thrown!"; });
  };
};
}  // namespace hal