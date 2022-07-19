#pragma once

#include "../counter/interface.hpp"
#include "../counter/util.hpp"
#include "../frequency.hpp"
#include "../input_pin/interface.hpp"
#include "../output_pin/interface.hpp"
#include "interface.hpp"

namespace hal {
/**
 * @brief SPI implementation using a generic output and input pin.
 *
 */
class bit_bang_spi : public hal::spi
{
public:
  bit_bang_spi(hal::counter& p_counter,
               hal::output_pin& p_data_out,
               hal::input_pin& p_data_in,
               hal::output_pin& p_clock)
    : m_counter(&p_counter)
    , m_data_out(&p_data_out)
    , m_data_in(&p_data_in)
    , m_clock(&p_clock)
  {
  }

private:
  boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept override
  {
    using namespace hal::literals;
    auto uptime = BOOST_LEAF_CHECK(m_counter->uptime());
    if (p_settings.clock_rate > uptime.frequency &&
        p_settings.clock_rate > 1_MHz) {
      return boost::leaf::new_error(hal::error::invalid_settings{});
    }
    m_settings = p_settings;
    return {};
  }
  boost::leaf::result<void> driver_transfer(
    [[maybe_unused]] std::span<const hal::byte> p_data_out,
    [[maybe_unused]] std::span<hal::byte> p_data_in,
    [[maybe_unused]] hal::byte p_filler) noexcept override
  {
    // check data transfer size
    // p_data_out.size() empty: write filler on bus
    // p_data_in.size() empty: ignore incoming data
    // p_data_out.size() < p_data_in.size(): send filler
    // p_data_out.size() > p_data_in.size(): ignore extra filler bytes

    // Loop the enough times to full read and write each byte
    auto in_byte = p_data_in.begin();
    auto out_byte = p_data_out.begin();
    while (in_byte != p_data_in.end() || out_byte != p_data_out.end()) {
      if (in_byte != p_data_in.end()) {
        if (out_byte != p_data_out.end()) {
          *in_byte = BOOST_LEAF_CHECK(transfer_byte(*out_byte));
          out_byte++;
        } else {
          *in_byte = BOOST_LEAF_CHECK(transfer_byte(p_filler));
        }
        in_byte++;
      } else {
        if (out_byte != p_data_out.end()) {
          BOOST_LEAF_CHECK(transfer_byte(*out_byte));
          out_byte++;
        }
      }
    }
  }

  boost::leaf::result<hal::byte> transfer_byte(hal::byte p_byte_out)
  {
    auto uptime = BOOST_LEAF_CHECK(m_counter->uptime());
    auto wavelength = uptime.frequency.wavelength();
    auto delay_time =
      std::chrono::duration_cast<hal::time_duration>(wavelength);
    BOOST_LEAF_CHECK(
      hal::delay(*m_counter, delay_time));  // DELAY LINE FOR MY REFERENCE

    hal::byte byte_in{ 0 };

    for (int bit = 0; bit < 8; bit++) {
      // READ
      // Set data_out bit
      m_data_out->level((p_byte_out >> bit) & 1);
      // delay
      hal::delay(*m_counter, delay_time);
      // toogle clock to active
      m_clock->level(!m_settings.clock_idles_high);
      // WRITE
      // read data_in bit
      if (m_data_in.level()) {
        byte_in |= 1 << bit;
      } else {
        byte_in &= !(1 << bit);
      }
      // delay
      hal::delay(*m_counter, delay_time);
      // toogle clock to reset/idle
      m_clock->level(m_settings.clock_idles_high);
    }
    return byte_in;
  }

  hal::counter* m_counter;
  hal::output_pin* m_data_out;
  hal::input_pin* m_data_in;
  hal::output_pin* m_clock;

  hal::spi::settings m_settings{};
};
}  // namespace hal
