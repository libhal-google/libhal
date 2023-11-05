#include "helpers.hpp"

bool compare_floats(float p_first,         // NOLINT
                    float p_second,        // NOLINT
                    float p_error_margin)  // NOLINT
{
  float difference = std::abs(p_first - p_second);
  return difference < p_error_margin;
}