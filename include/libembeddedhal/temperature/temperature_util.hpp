#pragma once

#include <array>

#include "temperature.hpp"

namespace embed {
/**
 * @brief Read N number of samples from the temperature sensor.
 *
 * This function will block until all samples have been read into the buffer.
 *
 * @tparam SampleSize - number of samples to read from the device
 * @param p_temperature_sensor - temperature sensor implementation
 * @return boost::leaf::result<std::array<temperature, SampleSize>> - array of
 * temperature data or an error.
 */
template<size_t SampleSize = 1>
[[nodiscard]] inline boost::leaf::result<std::array<temperature, SampleSize>>
read(temperature_sensor& p_temperature_sensor)
{
  std::array<temperature, SampleSize> samples;
  BOOST_LEAF_CHECK(p_temperature_sensor.read(samples));
  return samples;
}
}  // namespace embed
