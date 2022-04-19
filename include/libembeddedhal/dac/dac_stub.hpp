#include "dac.hpp"

namespace embed::stub {
/**
 * @brief A stub implementation of embed::dac to be used as a placeholder for
 * drivers that require an optional dac. This implementation should only be used
 * when a driver has stated in its documentation that the dac is optional and
 * can be stubbed out.
 *
 */
class dac : public embed::dac
{
private:
  boost::leaf::result<void> driver_write(
    [[maybe_unused]] percent p_value) noexcept
  {
    return {};
  }
};
}  // namespace embed::stub
