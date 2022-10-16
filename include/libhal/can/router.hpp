#pragma once

#include <cstdint>
#include <memory_resource>
#include <unordered_map>

#include "interface.hpp"

namespace hal {
/**
 * @addtogroup can
 * @{
 */
/**
 * @brief Route CAN messages received on the can bus to callbacks based on ID.
 *
 */
class can_router
{
public:
  /**
   * @brief Construct a new can message router
   *
   * @param p_can - can peripheral to route messages for
   * @param p_memory_resource - memory resource used for storing callbacks
   */
  can_router(hal::can& p_can,
             std::pmr::memory_resource& p_memory_resource) noexcept
    : m_can(&p_can)
    , m_messages(&p_memory_resource)
  {
  }

  /**
   * @brief Setup a callback for when a particular ID is received
   *
   *
   * @param p_id - Associated ID of messages to be stored.
   * @param p_handler - callback to be executed when a p_id message is received.
   * @return status - error or success
   * @throws std::errc::not_enough_memory if the internal network map does not
   * have enough space to add another handler.
   */
  [[nodiscard]] status on_receive(
    hal::can::id_t p_id,
    std::function<hal::can::handler> p_handler) noexcept
  {
    auto result = m_messages.emplace(std::make_pair(p_id, p_handler));
    auto was_successful = result.second;

    if (!was_successful) {
      return hal::new_error(std::errc::not_enough_memory);
    }

    if (!m_initialized) {
      m_initialized = true;
      HAL_CHECK(m_can->on_receive([this](const can::message_t& p_message) {
        auto router_pair = m_messages.find(p_message.id);
        if (router_pair != m_messages.end()) {
          router_pair->second(p_message);
        }
      }));
    }

    return hal::success();
  }

  /**
   * @brief get a reference to the can peripheral driver
   *
   * Can be used to initialize, configure, and enable the peripheral as well as
   * transmit messages.
   *
   * @return can& reference to the can peripheral driver
   */
  [[nodiscard]] hal::can& bus() noexcept
  {
    return *m_can;
  }

  /**
   * @brief Get the Internal Map object
   *
   * Meant for testing purposes or when direct inspection of the map is useful
   * in userspace. Should not be used in by libraries.
   *
   * @return const auto& map of all of the messages in the network.
   */
  [[nodiscard]] const auto& get_internal_map() noexcept
  {
    return m_messages;
  }

private:
  hal::can* m_can{};
  std::pmr::unordered_map<hal::can::id_t, std::function<hal::can::handler>>
    m_messages{};
  bool m_initialized = false;
};
/** @} */
}  // namespace hal
