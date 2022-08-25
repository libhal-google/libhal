#pragma once

#include <algorithm>

#include "math.hpp"

namespace hal {
/**
 * @addtogroup utility
 * @{
 */
/**
 * @brief Map an target value [x] from an input range [a,b] to an output range
 * [c,d].
 *
 * Another term for this is an affine transformation which follows this
 * equation:
 *
 *                       / d - c \
 *        y = (x - a) * | --------| + c
 *                       \ b - a /
 *
 * For example:
 *
 *    let x = 5.0
 *    let input_range = [0.0, 10.0]
 *    let output_range = [100.0, 200.0]
 *    The result will be 150.0
 *
 * @param p_target - target value within p_input_range to be mapped to the
 * output range.
 * @param p_input_range - the input range of p_target
 * @param p_output_range - the output range to map p_target to
 * @return constexpr float - value mapped from input range to the output
 * range. The output is clamped to the output range.
 */
[[nodiscard]] constexpr float map(
  float p_target,
  std::pair<float, float> p_input_range,
  std::pair<float, float> p_output_range) noexcept
{
  /**
   *                / d - c \
   * y = (x - a) * | --------| + c
   *                \ b - a /
   */
  const float x = p_target;
  const auto [a, b] = std::minmax(p_input_range.first, p_input_range.second);
  const auto [c, d] = std::minmax(p_output_range.first, p_output_range.second);

  const float shift_input = x - a;
  const float ratio = (d - c) / (b - a);
  const float y = (shift_input * ratio) + c;
  const float clamped_y = std::clamp(y, c, d);

  return clamped_y;
}
/** @} */
}  // namespace hal
