#include <cmath>

#include "helpers.hpp"

bool compare_floats(compare_float_t p_compare)
{
  float difference = std::abs(p_compare.a - p_compare.b);
  return difference < p_compare.margin;
}