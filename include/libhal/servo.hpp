#pragma once

#include "config.hpp"
#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief Hardware abstraction for a closed loop position controlled rotational
 * actuator
 *
 */
class servo
{
public:
  /**
   * @brief Feedback from setting the servo position
   *
   * This structure is currently empty as no feedback has been determined for
   * now. This structure may be expanded in the future.
   */
  struct position_t
  {};

  /**
   * @brief Error information indicating the range of the servo
   *
   */
  struct range_error
  {
    /**
     * @brief Minimum range of the servo shaft in degrees
     *
     */
    hal::degrees min;
    /**
     * @brief Maximum range of the servo shaft in degrees
     *
     */
    hal::degrees max;
  };

  /**
   * @brief Set the position of the servo's output shaft
   *
   * Position is the rotational position as a angle in degrees that the caller
   * wants the shaft to rotate to. The allowed range of positions is defined by
   * the servo itself. Many servos have intrinsic limits to their range.
   *
   * Developers must choose servos that fit the range for their applications.
   * Applications must clearly define the range that they require in order to
   * perform correctly.
   *
   * The velocity in which the servo shaft moves is not defined by this function
   * but is either intrinsic to the servo or a configuration of the servo.
   *
   * @param p_position - the position to move the servo shaft in degrees.
   * @return result<position_t> - success or failure
   * @throws std::errc::invalid_argument - when position exceeds the range of
   * the servo. When this error occurs, the guaranteed behavior is that the
   * servo keeps its last set position.
   * @throws hal::servo::range_error - when position exceeds the range of
   * the servo. Provides details about the min and max range of the servo. When
   * this error occurs, the guaranteed behavior is that the servo keeps its last
   * set position.
   */
  [[nodiscard]] result<position_t> position(hal::degrees p_position)
  {
    return driver_position(p_position);
  }

  virtual ~servo() = default;

private:
  virtual result<position_t> driver_position(hal::degrees p_power) = 0;
};
}  // namespace hal
