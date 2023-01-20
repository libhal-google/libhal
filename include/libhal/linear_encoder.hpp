#pragma once

#include "config.hpp"
#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief
 */
class linear_encoder
{
public:
  struct read_t
  {
    meters distance;
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

  virtual ~linear_encoder() = default;

private:
  virtual result<read_t> driver_read() = 0;
};
}  // namespace hal
