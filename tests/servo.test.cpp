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
#include <libhal/servo.hpp>

#include <boost/ut.hpp>

namespace hal {
namespace {
constexpr auto expected_value = hal::degrees(45);

class test_servo : public hal::servo
{
public:
  hal::degrees m_position = 0.0f;
  ~test_servo() override = default;

private:
  position_t driver_position(hal::degrees p_position) override
  {
    m_position = p_position;
    return {};
  }
};
}  // namespace

void servo_test()
{
  using namespace boost::ut;
  "servo interface test"_test = []() {
    // Setup
    test_servo test;

    // Exercise
    test.position(expected_value);

    // Verify
    expect(that % expected_value == test.m_position);
  };
};
}  // namespace hal