#pragma once

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <memory_resource>
#include <unordered_map>
#include <utility>

#include "../can.hpp"
#include "../driver.hpp"

namespace embed {
/// can_network is a canbus message receiver handler and
class can_network : public embed::driver<>
{
public:
  /// The node stored in the can_network map. Holds the latest CAN message and
  /// contains methods for updating and retreiving can messages in a thread-safe
  /// manner that does not invoke OS locks.
  ///
  /// Updating the CAN message data is completely lock free. Retrieving
  /// data is NOT lock free, but instead uses atomics to poll for when the
  /// update() function has completed in some other thread. This asymmetry in
  /// locking is to reduce latency for write case rather than than read case.
  /// Storing a CAN message is typically done via an interrupt service routine
  /// or a thread that MUST NOT block in anyway or the system can lock up. Where
  /// as reading data typically is done by a userspace thread which can
  /// typically wait a few cycles to get its data.
  class node_t
  {
  public:
    /// Default constructor
    node_t() noexcept {}

    /// Node assignment operator
    node_t& operator=(const node_t& node) noexcept
    {
      data = node.data;
      access_counter.store(access_counter.load());
      return *this;
    }

    /// Copy constructor
    node_t(const node_t& node) noexcept { *this = node; }

    /// Return a CAN message, but only do so if the CAN message of this node is
    /// not currently be modified by another thread that is using the update()
    /// method.
    can::message secure_get()
    {
      // Continuously check if the received CAN message is valid. NOTE: that, in
      // general, the looping logic for this function almost never occurs as
      // copies from RAM/cache are fast between receive_handler and GetMessage()
      while (true) {
        // First, atomically copy the access_counter to a variable.
        const auto read_message_start = access_counter.load();

        // Copy the contents of the mesage into the kCanMessage variable.
        const can::message kCanMessage = data;

        // Finally, atomically copy the access_counter again such that the value
        // between the start and finish can be compared.
        const auto read_message_finish = access_counter.load();

        // If and only if these two values are the same, meaning that the
        // receive_handler isn't currently modifying the node_t's data,
        // can the message be considered valid.
        if (read_message_start == read_message_finish) {
          return kCanMessage;
        }
      }

      return {};
    };

  private:
    friend can_network;

    /// updates the can message in a lock-free way. Can only be accessed by the
    /// can_network class.
    ///
    /// @param new_data - New CAN message to store
    void update(const can::message& new_data)
    {
      // Atomic increment of the access counter to notify any threads that are
      // using GetMesage() that the value of this node is changing.
      access_counter++;

      // Copy the contents of the new message into the map.
      data = new_data;

      // Another atomic increment of the access counter to notify any thread
      // using GetMessage() that the value of this node has finished changing.
      // For GetMessage to be valid, the function must save the access_counter,
      // copy the message data, then check the access_counter again to make sure
      // it has not changed. If it has changed, then GetMessage() should
      // continue polling the map node until access_counter is the same between
      // copies.
      access_counter++;
    }

    /// Holds the latest received can message;
    can::message data = {};

    /// Used to indicate when the data field is being accessed
    std::atomic<int> access_counter = 0;
  };

  /// @param can - CAN peripheral to manage the network of.
  /// @param memory_resource - pointer to a memory resource.
  can_network(can& p_can, std::pmr::memory_resource& p_memory_resource) noexcept
    : m_can(p_can)
    , m_messages(&p_memory_resource)
  {}

  bool driver_initialize() override
  {
    if (!m_can.initialize()) {
      return false;
    }
    m_can.attach_interrupt([this](can& p_can) { receive_handler(p_can); });
    return true;
  }

  /// In order for a CAN message with an associated ID to be stored in the
  /// can_network, it must be declared using this method. For example if you
  /// expect to get the following IDs 0x140, 0x7AA, and 0x561 from the CAN bus,
  /// then this method must be called as such:
  ///
  /// ```
  ///    node_t * motor_node       = can_network.register_message_id(0x140);
  ///    node_t * encoder_node     = can_network.register_message_id(0x561);
  ///    node_t * temperature_node = can_network.register_message_id(0x7AA);
  /// ```
  ///
  /// @param id - Associated ID of messages to be stored.
  /// @throw std::bad_alloc if this static storage allocated for this object is
  /// not enough to hold
  /// @return node_t* - reference to the CANBUS network node_t which can be used
  /// at anytime to retreive the latest received message from the CANBUS that is
  /// associated with the set ID.
  [[nodiscard]] node_t* register_message_id(uint32_t id)
  {
    node_t empty_node;

    // Insert an empty node_t into the map with the following ID as
    // the key. This is how each ID is remembered when calls to
    // receive_handler() and GetMessage() are performed. receive_handler()
    // will ignore messages with IDs that are not already keys within the map.
    // GetMessage() will throw an exception.
    m_messages.emplace(std::make_pair(id, empty_node));

    // Return reference to the newly made node_t.
    return &m_messages[id];
  }

  /// Manually call the receive handler. This is useful for unit testing and for
  /// CANBUS peripherals that do NOT have a receive message interrupt routine.
  /// In the later case, a software (potentially a thread) can perform the
  /// receive call manually to extract messages from the CAN peripheral FIFO.
  /// This method cannot guarantee that data is not lost if the FIFO fills up.
  void manually_call_receive_handler() { receive_handler(m_can); }

  /// Return the CAN peripheral object which can be used to initialize,
  /// configure, and enable the peripheral as well as transmit messages.
  /// Access to this object, if a can_network
  can& bus() { return m_can; }

  /// Meant for testing purposes or when direct inspection of the map is useful
  /// in userspace. Should not be used in by libraries.
  const auto& GetInternalMap() { return m_messages; }

private:
  void receive_handler(can& p_can)
  {
    // If there isn't any data available, return early.
    if (!p_can.has_data()) {
      return;
    }

    // Pop the latest can message off the queue.
    const auto kMessage = p_can.receive();

    // Check if the map already has a value for this ID. This acts as the last
    // stage of the CAN filter for the CANBUS Network module. If the key
    // does NOT exist in the map, then this message will not be saved.
    // Typically, this only happens when the hardware filter has not been setup
    // properly to eliminate CAN messages that should be ignored.
    //
    // Map lookups can be costly, especially in a interrupt context, so only
    // needing to hash/lookup the ID once is preferred. To prevent multiple
    // lookups, an iterator is stored into the message_node variable.
    auto message_node = m_messages.find(kMessage.id);

    // If the ID has an associated value in the map, then the node std::pair<>
    // is returned, otherwise, the ::end() node is returned.
    if (message_node != m_messages.end()) {
      message_node->second.update(kMessage);
    }
  }

  can& m_can;
  std::pmr::unordered_map<uint32_t, node_t> m_messages;
};
} // namespace embed