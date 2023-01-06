#pragma once

#include <cstdint>

#include "config.hpp"

#include "third_party/inplace_function.hpp"
#include <tl/function_ref.hpp>

namespace hal {
template<typename F>
using function_ref = tl::function_ref<F>;
template<typename F, size_t Capacity>
using inplace_function = stdext::inplace_function<F, Capacity>;
template<typename F>
using callback = inplace_function<F, sizeof(std::intptr_t) * 2>;
}  // namespace hal
