#pragma once

#include "../math.hpp"
#include "interface.hpp"

namespace hal {
/**
 * @addtogroup i2c
 * @{
 */
/**
 * @brief A i2c wrapper to ensure that the lowest i2c device frequency is used.
 *
 */
class minimum_speed_i2c : public hal::i2c
{
public:
  constexpr static auto default_max_speed = 2'000'000;
  /**
   * @brief Factory function to create minimum_speed_i2c object.
   *
   * @param p_i2c - i2c object that the device will use
   * @param p_frequency - the maximum starting frequency the device can use
   * @return minimum_speed_i2c - the configured i2c object using the lowest seen
   * frequency
   */
  static minimum_speed_i2c create(hal::i2c& p_i2c,
                                  hertz p_frequency = default_max_speed)
  {
    return minimum_speed_i2c(p_i2c, p_frequency);
  }

private:
  constexpr minimum_speed_i2c(hal::i2c& p_i2c, hertz p_frequency)
    : m_i2c(&p_i2c)
    , m_lowest_seen_frequency(p_frequency)
  {
  }

  status driver_configure(const settings& p_new_setting) noexcept override
  {
    if (equals(p_new_setting.clock_rate, 0.0_Hz)) {
      return hal::new_error(std::errc::invalid_argument);
    }
    if (m_lowest_seen_frequency > p_new_setting.clock_rate) {
      m_lowest_seen_frequency = p_new_setting.clock_rate;
      return m_i2c->configure(p_new_setting);
    }
    return {};
  }

  status driver_transaction(
    hal::byte p_address,
    std::span<const hal::byte> p_data_out,
    std::span<hal::byte> p_data_in,
    std::function<hal::timeout_function> p_timeout) noexcept override
  {
    return m_i2c->transaction(p_address, p_data_out, p_data_in, p_timeout);
  }

  hal::i2c* m_i2c;
  hertz m_lowest_seen_frequency;
};
/** @} */
}  // namespace hal
