# Copyright 2023 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.15)

# Because libhal is a header only library, this cmake file only builds and
# executes the unit tests
project(libhal LANGUAGES CXX)

libhal_unit_test(SOURCES
  tests/helpers.cpp
  tests/can.test.cpp
  tests/pwm.test.cpp
  tests/timer.test.cpp
  tests/i2c.test.cpp
  tests/spi.test.cpp
  tests/adc.test.cpp
  tests/dac.test.cpp
  tests/input_pin.test.cpp
  tests/interrupt_pin.test.cpp
  tests/output_pin.test.cpp
  tests/serial.test.cpp
  tests/steady_clock.test.cpp
  tests/motor.test.cpp
  tests/timeout.test.cpp
  tests/error.test.cpp
  tests/accelerometer.test.cpp
  tests/distance_sensor.test.cpp
  tests/gyroscope.test.cpp
  tests/magnetometer.test.cpp
  tests/rotation_sensor.test.cpp
  tests/temperature_sensor.test.cpp
  tests/servo.test.cpp
  tests/g_force.test.cpp
  tests/lengths.test.cpp
  tests/main.test.cpp

  PACKAGES
  Boost
  tl-function-ref

  LINK_LIBRARIES
  Boost::headers
  tl::function-ref)
