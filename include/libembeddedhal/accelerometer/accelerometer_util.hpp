#pragma once

#include <array>

#include "accelerometer.hpp"

namespace embed {
/**
 * @brief [WIP] Read N number of samples from the accelerometer.
 *
 * This function will block until all samples have been read into the buffer.
 *
 * @tparam SampleSize - number of samples to read from the device
 * @param p_accelerometer - accelerometer implementation
 * @return boost::leaf::result<
 * std::array<accelerometer::sample, SampleSize>> - array of accelerometer
 * samples or an error.
 */
template<size_t SampleSize = 1>
[[nodiscard]] inline boost::leaf::result<
  std::array<accelerometer::sample, SampleSize>>
read(accelerometer& p_accelerometer)
{
  std::array<accelerometer::sample, SampleSize> samples;
  BOOST_LEAF_CHECK(p_accelerometer.read(samples));
  return samples;
}
}  // namespace embed
