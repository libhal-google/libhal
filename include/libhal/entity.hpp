#pragma once

#include <array>
#include <cstdint>
#include <type_traits>

#include "error.hpp"
#include "units.hpp"

namespace hal {
/**
 * @brief
 *
 * @tparam storage_key_t
 * @tparam T
 * @tparam U
 */
template<class storage_key_t, class T, auto... U>
class entity
{
public:
  entity(T* p_ref)
  {
    m_pointer = p_ref;
  }

  T* operator->() const noexcept
  {
    return m_pointer;
  };

  T* operator->() noexcept
  {
    return m_pointer;
  }

  T const& operator*() const noexcept
  {
    return *m_pointer;
  }

  T& operator*() noexcept
  {
    return *m_pointer;
  }

  T& ref() noexcept
  {
    return *m_pointer;
  }

  T* ptr() noexcept
  {
    return m_pointer;
  }

  operator T*() noexcept
  {
    return m_pointer;
  }

private:
  inline static T* m_pointer = nullptr;
};

/**
 * @brief Get the static instance memory object
 *
 * @tparam storage_key
 * @tparam U
 * @return std::array<std::uint8_t, sizeof(U)>&
 */
template<typename storage_key, class U>
std::array<std::uint8_t, sizeof(U)>& get_static_instance_memory()
{
  static std::array<std::uint8_t, sizeof(U)> instance_memory{};
  return instance_memory;
}

/**
 * @brief
 *
 * @tparam T
 * @tparam Args
 * @tparam decltype([] {})
 * @param args
 * @return result<entity<storage_key, T>>
 */
template<class T, typename storage_key = decltype([] {}), typename... Args>
result<entity<storage_key, T>> initialize(Args... args)
{
  // Create a status variable to check if the constructor was successful with
  // setup
  hal::status constructor_status = hal::success();

  // Acquire the statically allocated buffer equal to the size of the object,
  // instance memory, based on the initializer ID and the T.
  auto& instance_memory = get_static_instance_memory<storage_key, T>();

  // In place construct the object at the instance memory location.
  //
  // NOTE: that no dynamic memory is NOT being used here. The statically
  // allocated memory is being used in place of heap. No calls to malloc are
  // made.
  new (instance_memory.data()) T(constructor_status, args...);

  // Check if the constructor had an error and if so, relay it up.
  if (!constructor_status) {
    return constructor_status.error();
  }

  // Return a pointer to the instance memory to hal::entity so it can store
  // the pointer in a statically allocated memory address.
  return reinterpret_cast<T*>(instance_memory.data());
}

/**
 * @brief
 *
 * @tparam T
 * @tparam storage_key
 * @tparam Args
 * @param p_entity
 * @param args
 * @return hal::status
 */
template<class T, typename storage_key, typename... Args>
hal::status reinitialize(entity<storage_key, T> p_entity, Args... args)
{
  // Execute destructor if it exists
  if constexpr (std::is_destructible_v<T>) {
    p_entity.ref().~T();
  }

  hal::status constructor_status = hal::success();
  new (p_entity.ptr()) T(constructor_status, args...);
  return constructor_status;
}
}  // namespace hal

/**
 * @brief
 *
 */
#define HAL_ENABLE_INITIALIZER                                                 \
  template<class T, typename storage_key, typename... Args>                    \
  friend hal::result<hal::entity<storage_key, T>> hal::initialize(             \
    Args... args)

/**
 * @brief
 *
 */
#define HAL_ENABLE_REINITIALIZER                                               \
  template<class T, typename storage_key, typename... Args>                    \
  friend hal::status hal::reinitialize(hal::entity<storage_key, T> p_entity,   \
                                       Args... args)
