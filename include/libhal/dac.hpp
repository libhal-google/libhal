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

#pragma once

#include <cstdint>

#include "config.hpp"
#include "error.hpp"

namespace hal {
/**
 * @brief Digital to Analog Converter (DAC) hardware abstraction interface.
 *
 * Use this interface for devices and peripherals that can create arbitrary
 * analog voltages between a defined Vss (negative reference) and Vcc (positive
 * reference) voltage.
 *
 */
class dac
{
public:
  /**
   * @brief Feedback from writing a voltage to the dac.
   *
   * This structure is currently empty as no feedback has been determined for
   * now. This structure may be expanded in the future.
   */
  struct write_t
  {};

  /**
   * @brief Set the output voltage of the DAC.
   *
   * The input value `p_percentage` is a 32-bit floating point value from 0.0f
   * to +1.0f.
   *
   * The floating point value is linearly proportional to the output voltage
   * relative to the Vss and Vcc such that if Vss is 0V (gnd) and Vcc is 5V then
   * 0.0 is 0V, 0.25 is 1.25V, 0.445 is 2.225V and 1.0 is 5V.
   *
   * This function clamps the input value between 0.0f and 1.0f and thus values
   * passed to driver implementations are guaranteed to be within this range.
   * Callers of this function do not need to clamp their values before passing
   * them into this function as it would be redundant. The rationale for doing
   * this at the interface layer is that it allows callers and driver
   * implementors to omit redundant clamping code, reducing code bloat.
   *
   * @param p_percentage - value from 0.0f to +1.0f representing the proportion
   * of the output voltage from the Vss to Vcc.
   * @return result<write_t> - success or failure
   */
  [[nodiscard]] result<write_t> write(float p_percentage)
  {
    auto clamped_percentage = std::clamp(p_percentage, 0.0f, 1.0f);
    return driver_write(clamped_percentage);
  }

  virtual ~dac() = default;

private:
  virtual result<write_t> driver_write(float p_percentage) = 0;
};
}  // namespace hal
