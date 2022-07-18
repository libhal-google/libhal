#pragma once

#include "../error.hpp"
#include "unit.hpp"

namespace embed {
/**
 * @brief Hardware abstraction interface for devices that report
 * position of a linear encoder.
 *
 */
class linear_encoder
{
public:
  /**
   * @brief Return the current position from the device
   *
   * The linear position can be relative, meaning that it will initialize
   * to a position of 0, and does not have information of its current
   * orientation. The linear position can also be absolute, meaning that the
   * encoder knows its current orientation at start and can return value a
   * non-zero value at initialization. This interface does not distinguish
   * between these two encoder schemes.
   *
   * @return boost::leaf::result<length> - current position reading or an error
   */
  [[nodiscard]] boost::leaf::result<length> read() noexcept
  {
    return driver_read();
  }

private:
  virtual boost::leaf::result<length> driver_read() noexcept = 0;
};
}  // namespace embed
