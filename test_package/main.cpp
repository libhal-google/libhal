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

#include <cstdio>
#include <system_error>

#include <libhal/error.hpp>
#include <libhal/pwm.hpp>

class test_pwm : public hal::pwm
{
private:
  hal::pwm::frequency_t driver_frequency(hal::hertz p_frequency) final
  {
    std::printf("frequency = %f Hz\n", p_frequency);

    return {};
  }
  hal::pwm::duty_cycle_t driver_duty_cycle(float p_position) final
  {
    m_error_count_down--;
    if (m_error_count_down == 0) {
      hal::safe_throw(std::errc::io_error);
    }

    std::printf("duty cycle = %f %%\n", p_position);

    return {};
  }

  int m_error_count_down = 2;
};

int main()
{
  using namespace hal::literals;

  int status = 0;
  test_pwm pwm;

  try {
    pwm.frequency(10.0_kHz);
    pwm.duty_cycle(0.25);
    pwm.duty_cycle(0.50);
    pwm.duty_cycle(-0.25);
    pwm.duty_cycle(-1.0);
  } catch (std::errc p_errc) {
    std::printf("Caught error successfully!\n");
    std::printf("    Error value: %d\n", static_cast<int>(p_errc));
  } catch (...) {
    std::printf("Unknown error!\n");
    status = -1;
  }

  return status;
}
