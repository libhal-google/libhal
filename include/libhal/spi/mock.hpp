
#pragma once

#include "../testing.hpp"
#include "../units.hpp"
#include "interface.hpp"

namespace hal::mock {
/**
 * @addtogroup spi
 * @{
 */
/**
 * @brief Mock spi implementation for use in unit tests and simulations with a
 * spy functions for configure() and a record for the transfer() out data. The
 * record ignores the in buffer and just stores the data being sent so it can be
 * inspected later.
 */
struct write_only_spi : public hal::spi
{
  /**
   * @brief Reset spy information for both configure() and transfer()
   *
   */
  void reset()
  {
    spy_configure.reset();
    write_record.clear();
  }

  /// Spy handler for hal::spi::configure()
  spy_handler<settings> spy_configure;
  /// Record of the out data from hal::spi::transfer()
  std::vector<std::vector<hal::byte>> write_record;

private:
  status driver_configure(const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  };
  status driver_transfer(std::span<const hal::byte> p_data_out,
                         [[maybe_unused]] std::span<hal::byte> p_data_in,
                         [[maybe_unused]] hal::byte p_filler) noexcept override
  {
    write_record.push_back({ p_data_out.begin(), p_data_out.end() });
    return {};
  };
};
/** @} */
}  // namespace hal::mock
