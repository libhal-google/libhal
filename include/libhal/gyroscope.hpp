#pragma once

#include "config.hpp"
#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief
 */
class gyroscope
{
public:
  struct read_t
  {
    rpm x;
    rpm y;
    rpm z;
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

  virtual ~gyroscope() = default;

private:
  virtual result<read_t> driver_read() = 0;
};
}  // namespace hal
