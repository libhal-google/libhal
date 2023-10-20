#include <cmath>

#include "helpers.hpp"

// NOLINTNEXTLINE
bool compare_floats(float p_first, float p_second, float p_error_margin)
{
  float difference = std::abs(p_first - p_second);
  return difference < p_error_margin;
}