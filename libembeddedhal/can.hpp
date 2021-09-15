#pragma once

#include "driver.hpp"

#include <array>
#include <chrono>
#include <cinttypes>
#include <functional>

namespace embed {
struct can_settings
{
  uint32_t clock_rate_hz = 100'000;
};

class can : public driver<can_settings>
{
  struct message
  {
    uint32_t id;
    uint8_t length = 0;
    std::array<uint8_t, 8> payload;
    bool is_remote_request = false;
  };

  virtual void send(const message& message) = 0;
  virtual message receive() = 0;
  virtual bool has_data() = 0;
  virtual void attach_interrupt(std::function<void(can&)> recieve_handler) = 0;
};
}
