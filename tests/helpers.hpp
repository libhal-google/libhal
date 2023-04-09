#pragma once
#include <cmath>

/** @brief Compares two single floating point values, with in a given error margin.
* @param first_num first single  point value to compare
* @param second_num second floating point value to compare
* @param error_margin How precise the error of an error should be checked.
* @return If the two floats are equal within a margin of error
*/
bool compare_floats(float first_num, float second_num, float error_margin = 0.0001f) {
    float difference = abs(first_num - second_num);
    return difference < error_margin;
}