#pragma once

#include "config.hpp"
#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief
 */
class accelerometer
{
public:
  struct read_t
  {
    g_force x;
    g_force y;
    g_force z;
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

  virtual ~accelerometer() = default;

private:
  virtual result<read_t> driver_read() = 0;
};
}  // namespace hal
