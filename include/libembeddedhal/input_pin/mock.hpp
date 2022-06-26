#include "../testing.hpp"
#include "interface.hpp"

namespace embed::mock {
/**
 * @brief Mock input_pin implementation for use in unit tests and simulations.
 *
 */
struct input_pin : public embed::input_pin
{
  /**
   * @brief Reset spy information for configure()
   *
   */
  void reset() { spy_configure.reset(); }

  /// Spy handler for embedd:input_pin::configure()
  spy_handler<settings> spy_configure;

  input_pin(bool p_level)
    : m_p_level(p_level){};
  /**
   * @brief Set input value of pin to given value
   *
   * @param p_level - high or low value for input pin
   */
  void set(bool p_level) { m_p_level = p_level; }

private:
  boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept override
  {
    return spy_configure.record(p_settings);
  };
  boost::leaf::result<bool> driver_level() noexcept { return m_p_level; }

  bool m_p_level;
};
}  // namespace embed::mock