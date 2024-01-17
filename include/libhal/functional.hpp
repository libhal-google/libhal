// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <cstdint>

#include "third_party/inplace_function.hpp"
#include <tl/function_ref.hpp>

/**
 * @defgroup Functional Functional
 *
 */
namespace hal {
/**
 * @ingroup Functional
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
 * @ingroup Functional
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
 * @ingroup Functional
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
