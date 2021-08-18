#pragma once

#include <cinttypes>

#include "driver.hpp"
#include "full_scale.hpp"

namespace embed
{
class adc : public driver<>
{
  virtual full_scale<uint32_t> read() = 0;
};
} // namespace embed
