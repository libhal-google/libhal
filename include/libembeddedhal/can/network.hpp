#pragma once

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <memory_resource>
#include <unordered_map>
#include <utility>

#include "can.hpp"

namespace embed {
/**
 * @brief Manage, store, and organize messages received on the can bus.
 *
 * Drivers use this can bus peripheral manager rather than can directly. This is
 * to ensure that the correct can messages are filtered to the correct driver.
 *
 * To understand the importance of using this class over can direction consider
 * protocols like UART and I2C. UART is asynchronous and communicates with a
 * single device. Messages may come at an arbitrary time, but their origin is
 * always known. Typically a single device driver holds control over a UART
 * peripheral. I2C is a multi device us but the controller is always the
 * initiator of the communication. Once an i2c controller successfully starts a
 * conversation with another device on the bus, the response should always come
 * from that device. CAN has this problem where messages can come in at any time
 * from any device on the bus, making writting a driver that accepts the can
 * interface directly impossible as there would be no way to coordinate which
 * driver gets what data when the can interface has only a singular receive
 * method.
 *
 * Can Network provides a means for routing messages based on ID to can device
 * drivers.
 *
 */
class can_network
{
public:
  /**
   * @brief A can network node stores the can messages in a lock free way.
   *
   * Updating the can message data is completely lock free. Retrieving data uses
   * atomics to poll for when the update() function has completed. This
   * asymmetry in locking is to reduce write time, which is usually
   * done in an interrupt context, rather than than read time, which is
   * performed by a driver in a thread or main thread.
   *
   */
  class node_t
  {
  public:
    /**
     * @brief Construct a new node t object
     *
     */
    node_t() noexcept {}

    /**
     * @brief Node assignment operator
     *
     * @param p_node the node copy
     * @return node_t& reference to this object
     */
    node_t& operator=(const node_t& p_node) noexcept
    {
      m_data = p_node.m_data;
      m_access_counter.store(m_access_counter.load());
      return *this;
    }

    /**
     * @brief Construct a new node t object
     *
     * @param p_node the object to copy
     */
    node_t(const node_t& p_node) noexcept { *this = p_node; }

    /**
     * @brief Get this node's can message
     *
     * @return can::message_t
     */
    [[nodiscard]] can::message_t secure_get() noexcept
    {
      // Continuously check if the received CAN message is valid. NOTE: that, in
      // general, the looping logic for this function almost never occurs as
      // copies from RAM/cache are fast between receive_handler and GetMessage()
      while (true) {
        // First, atomically copy the access_counter to a variable.
        const auto read_message_start = m_access_counter.load();

        // Copy the contents of the mesage into the can_message variable.
        const can::message_t can_message = m_data;

        // Finally, atomically copy the access_counter again such that the value
        // between the start and finish can be compared.
        const auto read_message_finish = m_access_counter.load();

        // If and only if these two values are the same, meaning that the
        // receive_handler isn't currently modifying the node_t's data,
        // can the message be considered valid.
        if (read_message_start == read_message_finish) {
          return can_message;
        }
      }

      return {};
    };

  private:
    friend can_network;

    /**
     * @brief Update can message
     *
     * This update is performed in a lock-free way and can only be accessed by
     * the can_network class.
     *
     * @param p_new_data New can message to store
     */
    void update(const can::message_t& p_new_data) noexcept
    {
      // Atomic increment of the access counter to notify any threads that are
      // using GetMesage() that the value of this node is changing.
      m_access_counter++;

      // Copy the contents of the new message into the map.
      m_data = p_new_data;

      // Another atomic increment of the access counter to notify any thread
      // using GetMessage() that the value of this node has finished changing.
      // For GetMessage to be valid, the function must save the access_counter,
      // copy the message data, then check the access_counter again to make sure
      // it has not changed. If it has changed, then GetMessage() should
      // continue polling the map node until access_counter is the same between
      // copies.
      m_access_counter++;
    }

    /// Holds the latest received can message
    can::message_t m_data = {};

    /// Used to indicate when the data field is being accessed
    std::atomic<int> m_access_counter = 0;
  };

  /**
   * @brief Construct a new can network object
   *
   * @param p_can - can peripheral to manage the network of
   * @param p_memory_resource - memory resource used for storing can messages
   */
  can_network(can& p_can, std::pmr::memory_resource& p_memory_resource) noexcept
    : m_can(&p_can)
    , m_messages(&p_memory_resource)
  {
    auto handler = [this](const can::message_t& p_message) {
      receive_handler(p_message);
    };
    if (!m_can->attach_interrupt(handler)) {
      std::abort();
    }
  }

  /**
   * @brief Assocate a can id with a node in the can network.
   *
   * To store can message with an associated ID in the can_network, it must be
   * declared using this method. For example if you expect to get the following
   * IDs 0x140, 0x7AA, and 0x561 from the CAN bus, then this method must be
   * called as such:
   *
   * ```C++
   * node_t* motor_node = can_network.register_message_id(0x140);
   * node_t* encoder_node = can_network.register_message_id(0x561);
   * node_t* temperature_node = can_network.register_message_id(0x7AA);
   * ```
   *
   * @param p_id - Associated ID of messages to be stored.
   * @throw std::bad_alloc if this static storage allocated for this object is
   * not enough to hold
   * @return node_t* - reference to the CANBUS network node_t which can be used
   * at anytime to retreive the latest received message from the CANBUS that is
   * associated with the set ID.
   *
   */
  [[nodiscard]] node_t* register_message_id(can::id_t p_id) noexcept
  {
    node_t empty_node;

    // Insert an empty node_t into the map with the following ID as
    // the key. This is how each ID is remembered when calls to
    // receive_handler() and GetMessage() are performed. receive_handler()
    // will ignore messages with IDs that are not already keys within the map.
    // GetMessage() will throw an exception.
    m_messages.emplace(std::make_pair(p_id, empty_node));

    // Return reference to the newly made node_t.
    return &m_messages[p_id];
  }

  /**
   * @brief get a reference to the can peripheral driver
   *
   * Can be used to initialize, configure, and enable the peripheral as well as
   * transmit messages.
   *
   * @return can& reference to the can peripheral driver
   */
  [[nodiscard]] can& bus() noexcept { return *m_can; }

  /**
   * @brief Get the Internal Map object
   *
   * Meant for testing purposes or when direct inspection of the map is useful
   * in userspace. Should not be used in by libraries.
   *
   * @return const auto& map of all of the messages in the network.
   */
  [[nodiscard]] const auto& get_internal_map() noexcept { return m_messages; }

private:
  void receive_handler(const can::message_t& p_message) noexcept
  {
    // Check if the map already has a value for this ID. This acts as the last
    // stage of the CAN filter for the CANBUS Network module. If the key
    // does NOT exist in the map, then this message will not be saved.
    // Typically, this only happens when the hardware filter has not been setup
    // properly to eliminate CAN messages that should be ignored.
    //
    // Map lookups can be costly, especially in a interrupt context, so only
    // needing to hash/lookup the ID once is preferred. To prevent multiple
    // lookups, an iterator is stored into the message_node variable.
    auto message_node = m_messages.find(p_message.id);

    // If the ID has an associated value in the map, then the node std::pair<>
    // is returned, otherwise, the ::end() node is returned.
    if (message_node != m_messages.end()) {
      message_node->second.update(p_message);
    }
  }

  can* m_can{};
  std::pmr::unordered_map<uint32_t, node_t> m_messages{};
};
}  // namespace embed