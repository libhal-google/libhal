#pragma once

#include "config.hpp"

#include <tl/function_ref.hpp>

namespace hal {
template<typename F>
using function_ref = tl::function_ref<F>;
}  // namespace hal
