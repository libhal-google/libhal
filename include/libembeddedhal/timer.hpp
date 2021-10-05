#pragma once

#include "driver.hpp"
#include "inactive.hpp"

#include <chrono>
#include <cinttypes>
#include <cstddef>
#include <functional>

namespace embed {
class timer : public driver<>
{
public:
  enum class controls
  {
    start,
    stop,
    reset,
  };

  enum class type
  {
    oneshot,
    continuous,
  };

  virtual bool attach_interrupt(std::function<void(void)> p_callback,
                                std::chrono::nanoseconds interval,
                                type p_type = type::continuous) = 0;
  virtual bool control(controls p_control) = 0;
  virtual bool is_active() = 0;
};
} // namespace embed
