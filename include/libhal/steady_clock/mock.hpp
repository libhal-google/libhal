#pragma once

#include <queue>

#include "interface.hpp"

namespace hal::mock {
/**
 * @addtogroup steady_clock
 * @{
 */
// TODO(#419): Fill this out
/**
 * @brief mock steady_clock implementation for use in unit tests and
 * simulations.
 *
 */
struct steady_clock : public hal::steady_clock
{
private:
};
/** @} */
}  // namespace hal::mock
