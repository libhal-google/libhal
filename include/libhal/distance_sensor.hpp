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

#include "units.hpp"

namespace hal {
/**
 * @brief Linear distance hardware abstraction interface
 *
 * Examples of distance encoder are:
 *
 *    - Linear Potentiometers
 *    - LIDAR or TOF (time of flight) sensor
 *    - Ultrasonic range finder
 *    - Infrared Distance Sensors
 *    - Linear Quadrature Encoders
 *    - Linear Incremental Encoders
 *    - Linear Absolute Encoders
 *    - Linear Magnetic Encoders
 *
 * Distance sensors can be relative or absolute. Relative position means that
 * the sensor can only see changes in rotation from where measurement started.
 * In other words, at application start, relative encoders will start at 0.
 * Absolute encoders know their position at all times. At application start, the
 * absolute encoder will be able to determine its exact orientation relative to
 * a frame of reference when read.
 *
 * Examples of relative rotation sensors are:
 *
 *    - Quadrature Encoders
 *    - Incremental Encoders
 *
 * Examples of absolute rotation sensors are:
 *
 *    - Potentiometers
 *    - Absolute Encoders
 *    - Rotary Magnetic Encoders
 *    - IMUs
 *
 * Distance sensors can also be finite or infinite. Finite meaning that the
 * angle that can be reported is a fixed amount for the device. Infinite means
 * that the encoder can continue rotating and adding more to its angle reading
 * forever. Infinite rotation sensors tend to not have a physical stop that
 * limits how much they can be rotated.
 *
 * Examples of finite rotation sensors are:
 *
 *    - Potentiometers
 *    - Absolute Encoders
 *    - IMUs
 *
 * Examples of infinite rotation sensors are:
 *
 *    - Rotary Magnetic Encoders
 *    - Quadrature Encoders
 *    - Incremental Encoders
 *
 * This interface does not provide a means to determine these attributes of a
 * rotation sensor as this is an application architecture decision. Drivers that
 * implement this interface should document what kind of distance sensor it is
 * such that a developer can determine its applicability to their application.
 * The context of which sensor ought to be used for an application is solely
 * known at architecture definition time and software should not be expected to
 * at runtime, if the right type of rotation sensor was passed into the object.
 */
class distance_sensor
{
public:
  /**
   * @brief Result from sampling the distance sensor
   *
   */
  struct read_t
  {
    /**
     * @brief Encoder distance measurement in meters
     *
     */
    meters distance;
  };

  /**
   * @brief Read the current distance measured by the device
   *
   * @return read_t - distance data
   */
  [[nodiscard]] read_t read()
  {
    return driver_read();
  }

  virtual ~distance_sensor() = default;

private:
  virtual read_t driver_read() = 0;
};
}  // namespace hal
