#pragma once

#include <cstdint>

#include "config.hpp"

#include "third_party/inplace_function.hpp"
#include <tl/function_ref.hpp>

namespace hal {
/**
 * @brief Definition of a non-owning callable object
 *
 * Use this for passing a callable object to a function that the function does
 * not need to store in anyway. Best used for timeouts where a function simply
 * needs the callable during the runtime of the function and when the function
 * is over, the callable is no longer needed.
 *
 * This function is light weight in comparison to std::function, which is
 * allocating, or inplace_function.
 *
 * @tparam F - function type or call signature
 */
template<typename F>
using function_ref = tl::function_ref<F>;

/**
 * @brief Definition of a owning callable object
 *
 * Use this instead of function_ref when a callable object needs to be stored.
 *
 * @tparam F - function type or call signature
 * @tparam Capacity - storage capacity of the function in bytes. If a callable
 * object has a size greater than the capacity, then attempting to create an
 * inplace function with it will result in a compiler error.
 */
template<typename F, size_t Capacity>
using inplace_function = stdext::inplace_function<F, Capacity>;

/**
 * @brief Definition of a standard libhal owning callback object
 *
 * This is an inplace_function with its capacity set to two pointers. Callable
 * objects must fit within the size of two integers to be able to construct this
 * polymorphic callable object.
 *
 * @tparam F - function type or call signature
 */
template<typename F>
using callback = inplace_function<F, sizeof(std::intptr_t) * 2>;
}  // namespace hal
