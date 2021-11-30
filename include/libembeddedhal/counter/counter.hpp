#pragma once

#include "../driver.hpp"

#include <chrono>
#include <cinttypes>

namespace embed {
class counter : public driver<>
{
public:
  enum class controls
  {
    start,
    stop,
    reset,
  };

  virtual void period(std::chrono::nanoseconds p_period) = 0;
  virtual std::chrono::nanoseconds period() = 0;
  virtual void control(controls p_control) = 0;
  virtual bool is_active() = 0;
  virtual uint64_t count() = 0;
};
} // namespace embed
