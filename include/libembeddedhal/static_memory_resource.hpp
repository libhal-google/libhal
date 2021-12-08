#pragma once

#include <cstddef>
#include <memory_resource>

namespace embed {
/**
 * @brief The static_memory_resource is the polymorphic memory resource
 * allocator that allows libembeddedhal's libraries to use a C++ standard
 * container such as std::pmr::vector, std::pmr::string,
 * std::pmr::unordered_map, and more without requiring dynamic memory allocation
 * from the heap. It provides a simple way to create an allocator with built in
 * storage, with its size indicated by a single template parameter. It also
 * provides statistics regarding the amount of memory allocated such that
 * developers can determine if they need to reduce or increase the size of the
 * buffer. Memory statistics can also give programs the insight into how much
 * memory space is available and make decisions based on that. In the event that
 * the memory of the static allocator is exceeded, a std::bad_alloc excpetion is
 * thrown.
 *
 * USAGE:
 *
 *    // Make a memory resource with 1kB buffer
 *    static_memory_resource<1024> memory_resource;
 *    std::pmr::unordered_map<int, const char*> id_name_map(&memory_resource);
 *
 * @tparam BufferSize number of bytes to statically allocate for the memory
 * resource.
 */
template<size_t BufferSize>
class static_memory_resource : public std::pmr::memory_resource
{
public:
  static_memory_resource()
    : m_buffer{}
    , m_unallocated_memory(m_buffer.data())
    , m_resource(m_buffer.data(),
                 m_buffer.size(),
                 std::pmr::null_memory_resource())
  {}
  /**
   * @return constexpr std::size_t - the total number of bytes that this
   * allocator can allocate before throwing a std::bad_alloc exception.
   */
  constexpr std::size_t capacity() noexcept const { return BufferSize; }
  /**
   * @return std::size_t number of bytes that have already been allocated.
   */
  std::size_t memory_used() noexcept const
  {
    return m_unallocated_memory - m_buffer.data();
  }
  /**
   * @return int Bytes that have yet to be allocated from this allocator.
   */
  int memory_available() noexcept const { return capacity() - memory_used(); }

protected:
  /**
   * @brief Implemenation of the do_allocate() method for
   * std::pmr::memory_resource
   *
   * @param bytes number of bytes to allocate
   * @param alignment alignment limit for the allocation
   * @return void* address of the newly allocated buffer
   */
  void* do_allocate(std::size_t bytes, std::size_t alignment) override
  {
    // Request a pointer to unallocated memory from the
    // monotonic_m_bufferresource buffer.
    void* allocated_address = m_resource.allocate(bytes, alignment);

    // If the above call has not thrown a std::bad_alloc exception, then the
    // allocated address must contain a valid address from buffer. To get the
    // location of the unallocated memory, simply add the number of bytes to
    // allocate_address variable.
    m_unallocated_memory = static_cast<std::byte*>(allocated_address) + bytes;

    return allocated_address;
  }
  /**
   * @brief Implemenation of the do_deallocate() method for
   * std::pmr::memory_resource
   *
   * @param address address of space to be deallocated
   * @param bytes number of bytes to allocate
   * @param alignment alignment limit for the allocation
   */
  void do_deallocate(void* address,
                     std::size_t bytes,
                     std::size_t alignment) override
  {
    return m_resource.deallocate(address, bytes, alignment);
  }
  /**
   * @brief Implemenation of the do_is_equal() method for
   * std::pmr::memory_resource
   *
   * @param other another memory resource to
   * @return true are equal
   * @return false are not equal
   */
  bool do_is_equal(
    const std::pmr::memory_resource& other) const noexcept override
  {
    return m_resource.is_equal(other);
  }

private:
  std::array<std::byte, BufferSize> m_buffer;
  std::byte* m_unallocated_memory;
  std::pmr::monotonic_buffer_resource m_resource;
};
}  // namespace embed
