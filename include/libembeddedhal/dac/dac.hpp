#pragma once

#include <cinttypes>

#include "../driver.hpp"
#include "../full_scale.hpp"

namespace embed {
class dac : public driver<>
{
public:
  virtual void write(full_scale<uint32_t> value) = 0;
};
} // namespace embed
