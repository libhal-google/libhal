#pragma once

#include "../move_interceptor.hpp"
#include "../static_callable.hpp"
#include "../static_list.hpp"
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
class can_router : public move_interceptor<can_router>
{
public:
  friend class move_interceptor<can_router>;

  static constexpr auto noop =
    []([[maybe_unused]] const can::message_t& p_message) {};

  using message_handler = std::function<hal::can::handler>;

  struct route
  {
    hal::can::id_t id = 0;
    message_handler handler = noop;
  };

  using route_item = hal::static_list<route>::item;

  static result<can_router> create(hal::can& p_can)
  {
    can_router new_can_router(p_can);
    HAL_CHECK(p_can.on_receive(std::ref(new_can_router)));
    return new_can_router;
  }

  can_router() = delete;
  can_router(can_router& p_other_self) = delete;
  can_router& operator=(can_router& p_other_self) = delete;
  can_router(can_router&& p_other_self) = default;
  can_router& operator=(can_router&& p_other_self) = default;

  /**
   * @brief Get a reference to the can peripheral driver
   *
   * Used to send can messages through the same port that the can_router is
   * using.
   *
   * @return can& reference to the can peripheral driver
   */
  [[nodiscard]] hal::can& bus() noexcept
  {
    return *m_can;
  }

  /**
   * @brief Add a message route without setting the callback
   *
   * The default callback will do nothing and will drop the message.
   *
   * @param p_id - Associated ID of messages to be stored.
   * @return auto - route item from the linked list that must be stored stored
   * in a variable
   */
  [[nodiscard]] auto add_message_callback(hal::can::id_t p_id) noexcept
  {
    return m_handlers.push_back(route{
      .id = p_id,
    });
  }

  /**
   * @brief Set a callback for when messages with a specific ID is received
   *
   * @param p_id - Associated ID of messages to be stored.
   * @param p_handler - callback to be executed when a p_id message is received.
   * @return auto - route item from the linked list that must be stored stored
   * in a variable
   */
  [[nodiscard]] auto add_message_callback(hal::can::id_t p_id,
                                          message_handler p_handler) noexcept
  {
    return m_handlers.push_back(route{
      .id = p_id,
      .handler = p_handler,
    });
  }

  /**
   * @brief Get the list of handlers
   *
   * Meant for testing purposes or when direct inspection of the map is useful
   * in userspace. Should not be used in by libraries.
   *
   * @return const auto& map of all of the can message handlers.
   */
  [[nodiscard]] const auto& handlers() noexcept
  {
    return m_handlers;
  }

  /**
   * @brief Message routing interrupt service handler
   *
   * Searches the static list and finds the first ID associated with the message
   * and run's that route's callback.
   *
   * @param p_message - message received from the bus
   */
  void operator()(const can::message_t& p_message)
  {
    for (auto& list_handler : m_handlers) {
      if (p_message.id == list_handler.id) {
        list_handler.handler(p_message);
        return;
      }
    }
  }

private:
  /**
   * @brief Update the callback location if this object is moved
   *
   * @param p_old_self - the old version of the can_router
   */
  void intercept(can_router* p_old_self)
  {
    // Assume that if this succeeded in the create factory function, that it
    // will work this time
    (void)p_old_self->m_can->on_receive(std::ref(*this));
  }

  /**
   * @brief Construct a new can message router
   *
   * @param p_can - can peripheral to route messages for
   * @param p_memory_resource - memory resource used for storing callbacks
   */
  explicit can_router(hal::can& p_can) noexcept
    : m_can(&p_can)
  {
  }

  hal::static_list<route> m_handlers;
  hal::can* m_can;
};
/** @} */
}  // namespace hal
