#pragma once

#include "config.hpp"
#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief
 */
class magnetometer
{
public:
  struct read_t
  {
    gauss x;
    gauss y;
    gauss z;
  };

  /**
   * @brief
   *
   * @return result<read_t> -
   */
  [[nodiscard]] result<read_t> read()
  {
    return driver_read();
  }

  virtual ~magnetometer() = default;

private:
  virtual result<read_t> driver_read() = 0;
};
}  // namespace hal
