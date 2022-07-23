#pragma once

#include <array>
#include <cstddef>
#include <experimental/coroutine>
#include <iterator>
#include <memory>
#include <optional>
#include <span>
#include <type_traits>

namespace hal {

/**
 * @brief An allocator aware coroutine for creating resumable functions
 *
 * When the coroutine finishes, meaning a `co_return` was called, the resumable
 * will continue to return the last yield value it had, thus calling a finished
 * resumable has well defined behavior.
 *
 * @tparam T - return type of the resumable function, cannot be void
 */
template<typename T>
struct resumable
{
  struct promise_type;
  using handle_t = std::experimental::coroutine_handle<promise_type>;

  struct promise_type
  {
    resumable get_return_object()
    {
      return resumable(handle_t::from_promise(*this));
    }

    std::experimental::suspend_always initial_suspend()
    {
      return {};
    }

    std::experimental::suspend_always final_suspend() noexcept
    {
      return {};
    }

    void unhandled_exception()
    {
    }

    template<std::convertible_to<T> From>
    std::experimental::suspend_always yield_value(From&& from)
    {
      value = std::forward<From>(from);
      return {};
    }

    void return_void()
    {
    }

    template<class Allocator, typename... Args>
    void* operator new(size_t p_size,
                       std::allocator_arg_t,
                       Allocator* alloc,
                       [[maybe_unused]] Args&... args)
    {
      return alloc->allocate(p_size);
    }

    void operator delete(void*, size_t)
    {
    }

    T value;
  };

  resumable(handle_t p_handle)
    : m_coroutine_handle(p_handle)
  {
  }

  ~resumable()
  {
    m_coroutine_handle.destroy();
  }

  explicit operator bool()
  {
    return !m_coroutine_handle.done();
  }

  T operator()()
  {
    if (!m_coroutine_handle.done()) {
      m_coroutine_handle();
    }
    return std::move(m_coroutine_handle.promise().value);
  }

private:
  handle_t m_coroutine_handle;
};
}  // namespace hal
