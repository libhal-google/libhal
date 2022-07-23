#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace hal {

/**
 * @brief An Allocator that uses a pool of memory
 *
 * This allocator is monotonic and does not deallocate
 *
 * @tparam T - allocator type
 */
template<class T>
class allocator
{
public:
  using value_type = T;

  /**
   * @brief Construct a new allocator object
   *
   * @param p_memory - pool of memory to use for the allocator. This memory must
   * outlive this object.
   */
  allocator(std::span p_memory)
    : m_memory(p_memory)
  {
  }

  template<class U>
  constexpr allocator(const allocator<U, Size>&) noexcept
  {
  }

  /**
   * @brief Allocate a number of elements from the memory pool
   *
   * @param p_number_of_elements - element count
   * @return T* - address of the allocated memory
   */
  [[nodiscard]] T* allocate(std::size_t p_number_of_elements)
  {
    if (p_number_of_elements > m_memory.size()) {
      return nullptr;
    }

    T* result = m_memory.data();
    m_memory = m_memory.subspan(p_number_of_elements);
    return result;
  }

  /**
   * @brief Deallocator that does nothing
   *
   * @param p_address
   * @param p_elements
   */
  void deallocate([[maybe_unused]] T* p_address,
                  [[maybe_unused]] std::size_t p_elements) noexcept
  {
  }

  template<class T, size_t TSize, class U, size_t USize>
  friend bool operator==(const allocator<T, TSize>&, const allocator<U, USize>&)
  {
    return true;
  }

  template<class T, size_t TSize, class U, size_t USize>
  friend bool operator!=(const allocator<T, TSize>&, const allocator<U, USize>&)
  {
    return false;
  }

private:
  std::span<T> m_memory;
};

/**
 * @brief Owning static memory allocator
 *
 * @tparam T - allocator type
 * @tparam Size - number of elements
 */
template<class T, size_t Size>
class static_allocator : public hal::allocator
{
public:
  /**
   * @brief Construct a new static allocator object
   *
   */
  static_allocator()
    : allocator(m_buffer)
  {
  }

private:
  std::array<T, Size> m_buffer;
};
}  // namespace hal
