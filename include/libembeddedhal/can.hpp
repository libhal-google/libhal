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
public:
  struct message
  {
    uint32_t id;
    uint8_t length = 0;
    std::array<uint8_t, 8> payload{0};
    bool is_remote_request = false;
  };

  virtual void send(const message& p_message) = 0;
  virtual message receive() = 0;
  virtual bool has_data() = 0;

  /**
   * @brief Will attach an interrupt to this can driver such that, when a
   *     message is received, it will call the handler supplied
   *
   * @param p_receive_handler - Handler to be called when a message is received.
   *     If this is set to nullptr, then this function shall disable the can
   *     interrupt or replace the interrupt with a function that does nothing,
   *     but still clears the interrupt service routine.
   */
  virtual void attach_interrupt(
    std::function<void(can&)> p_receive_handler) = 0;
};
}
