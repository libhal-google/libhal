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
                    float p_error_margin = 0.0001f)
{
  float difference = abs(p_first - p_second);
  return difference < p_error_margin;
}