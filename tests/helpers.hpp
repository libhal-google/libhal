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

#include <cmath>

/**
 * @brief Compares two single floating point values, with in a given error
 * margin.
 *
 * @param p_first first single  point value to compare
 * @param p_second second floating point value to compare
 * @param p_error_margin How precise the error of an error should be checked.
 * @return true if the two floats are equal within a margin of error
 */
bool compare_floats(float p_first,
                    float p_second,
                    float p_error_margin = 0.0001f);