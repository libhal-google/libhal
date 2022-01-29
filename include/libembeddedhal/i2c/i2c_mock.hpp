#pragma once

#include <cstdint>
#include <functional>
#include <vector>

#include "../error.hpp"
#include "../testing.hpp"
#include "i2c.hpp"

namespace embed::mock {
class i2c : public embed::i2c
{
public:
  enum increment_rule
  {
    stay,
    increment,
  };

  enum operation
  {
    read,
    write,
  };

  struct response
  {
    increment_rule rule = increment_rule::stay;
  };

  using device_address = std::byte;
  using register_address = std::byte;
  using register_function = std::function<response(operation, std::byte&)>;
  using memory_map = std::unordered_map<register_address, register_function>;
  using i2c_bus_t = std::unordered_map<device_address, memory_map>;

  boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  }

  boost::leaf::result<void> driver_transaction(
    std::byte p_address,
    std::span<const std::byte> p_out,
    std::span<std::byte> p_in) noexcept override
  {
    auto device = bus.at(p_address);

    // Set the register address
    if (p_out.size() > 0) {
      m_register_pointer = p_out[0];
    }

    // Perform write operations on the data after the first register byte as
    // that byte is used to indicate the register address
    for (auto& const_data : p_out.subspan(1)) {
      std::byte data = const_data;
      update_pointer(device.at(m_register_pointer)(operation::write, data));
    }

    // Read contents of bus into return data
    for (auto& data : p_in) {
      update_pointer(device.at(m_register_pointer)(operation::read, data));
    }

    return {};
  }

  void reset() { bus.clear(); }

  /// Spy handler for embed::i2c::configure()
  spy_handler<const settings&> spy_configure;
  /// Access to
  i2c_bus_t bus;

private:
  void update_pointer(response p_response)
  {
    if (p_response.rule == increment_rule::increment) {
      auto reg_number = std::to_integer<uint8_t>(m_register_pointer);
      m_register_pointer = std::byte{ ++reg_number };
    }
  }

  std::byte m_register_pointer;
};
}  // namespace embed::mock
