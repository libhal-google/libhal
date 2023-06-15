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

namespace hal {
extern void adc_test();
extern void can_test();
extern void dac_test();
extern void error_test();
extern void i2c_test();
extern void input_pin_test();
extern void interrupt_pin_test();
extern void motor_test();
extern void output_pin_test();
extern void pwm_test();
extern void serial_test();
extern void spi_test();
extern void steady_clock_test();
extern void timeout_test();
extern void timer_test();
extern void servo_test();
extern void g_force_test();
extern void lengths_test();
}  // namespace hal

int main()
{
  hal::adc_test();
  hal::can_test();
  hal::dac_test();
  hal::error_test();
  hal::i2c_test();
  hal::input_pin_test();
  hal::interrupt_pin_test();
  hal::motor_test();
  hal::output_pin_test();
  hal::pwm_test();
  hal::serial_test();
  hal::spi_test();
  hal::steady_clock_test();
  hal::servo_test();
  hal::timeout_test();
  hal::timer_test();
  hal::g_force_test();
  hal::lengths_test();
}
