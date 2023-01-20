#pragma once

#include "config.hpp"
#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief
 */
class rotary_encoder
{
public:
  struct read_t
  {
    degrees angle;
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

  virtual ~rotary_encoder() = default;

private:
  virtual result<read_t> driver_read() = 0;
};
}  // namespace hal
