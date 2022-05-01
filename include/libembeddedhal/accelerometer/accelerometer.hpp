#pragma once

#include <cstdint>
#include <span>

#include "../error.hpp"
#include "../percent.hpp"
#include "accelerometer_units.hpp"

namespace embed {
/**
 * @brief Accelerometer hardware abstraction interface.
 *
 * This interface represents devices and peripherals that have the capability to
 * measure acceleration.
 *
 */
class accelerometer
{
public:
  /**
   * @brief Representation of a sample of accelerometer data
   *
   * embed::percent was choosen for representing the x, y, and z data of an
   * accelerometer because conversion from N-bit register to percent is fast
   * compared to the math required to convert it to
   * embed::nanometre_per_second_sq. This means that multiple samples can be
   * gathered quickly to be converted to data at a later time.
   *
   */
  struct sample
  {
    /// Represents the maximum amplitude of acceleration for this sample of
    /// data. In many cases this is 1g, 2g, 4g, 8g and many other values, where
    /// "g" represents the acceleration due to gravity on Earth at sea level.
    /// For example, if full_scale is 2g and the X-axis returns 50%, then the
    /// acceleration read on X is 1g of acceleration.
    ///
    /// full_scale is made available for each sample in the event that the
    /// driver changes the full_scale between samples. This is uncommon and many
    /// applications will simply save the full scale once and drop saving it for
    /// subsequent calls.
    embed::acceleration full_scale;

    /// Represents the percentage of acceleration in the X, Y & Z axis relative
    /// to the full-scale
    struct axis_t
    {
      /// Percentage of acceleration in the X-axis relative to the full-scale
      percent x;
      /// Percentage of acceleration in the Y-axis relative to the full-scale
      percent y;
      /// Percentage of acceleration in the Z-axis relative to the full-scale
      percent z;

      /**
       * @brief Default operators for <, <=, >, >= and ==
       *
       * @return auto - result of the comparison
       */
      [[nodiscard]] constexpr auto operator<=>(const axis_t&) const noexcept =
        default;
    };

    /// Acceleration in the XYZ axis
    axis_t axis;

    /**
     * @brief Default operators for <, <=, >, >= and ==
     *
     * @return auto - result of the comparison
     */
    [[nodiscard]] constexpr auto operator<=>(const sample&) const noexcept =
      default;
  };

  /**
   * @brief Read the current acceleration from the accelerometer.
   *
   * @return boost::leaf::result<sample> - a sample of data or an error
   */

  /**
   * @brief Read out acceleration data from the accelerometer.
   *
   * For accelerometers not configured to store data within a FIFO or do not
   * have a FIFO available, this function will always return a span of data
   * length 1 sample. For multiple samples multiple calls of this function will
   * need to be made. This is the low bandwidth situation.
   *
   * If an accelerometer is configured to use a FIFO and utilizes interrupts to
   * detect when the FIFO is almost/entirely full, then a circular buffer shall
   * be used to store this data until read by this function. The length of the
   * returned span will be minimum between the p_samples buffer or the circular
   * buffer.
   *
   * @param p_samples - a buffer to fill with sample data
   * @return boost::leaf::result<std::span<sample>> - an error or a span of
   * sample data with the address equal to the p_samples argument and size, the
   * number of samples filled into the buffer.
   */
  [[nodiscard]] boost::leaf::result<std::span<sample>> read(
    std::span<sample> p_samples) noexcept
  {
    return driver_read(p_samples);
  }

private:
  virtual boost::leaf::result<std::span<sample>> driver_read(
    std::span<sample> p_samples) noexcept = 0;
};
}  // namespace embed
