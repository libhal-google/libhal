#pragma once

#include <array>
#include <cstdint>

#include "../error.hpp"
#include "../units.hpp"

namespace hal {
constexpr std::uint64_t application_domain(std::uint64_t p_storage_key)
{
  return p_storage_key + (1ULL << 32ULL);
}

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

template<class T>
class static_initializer
{
public:
  template<typename... Args, auto storage_key = [] {}>
  static result<entity<decltype(storage_key), T>> initialize(Args... args)
  {
    // Create a status variable to check if the constructor was successful with
    // setup
    hal::status constructor_status = hal::success();

    // Acquire the statically allocated buffer equal to the size of the object,
    // instance memory, based on the initializer ID and the T.
    // decltype(storage_key)
    auto& instance_memory =
      get_static_instance_memory<decltype(storage_key), T>();

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

  static_initializer() = default;
  static_initializer(static_initializer&) = delete;
  static_initializer(static_initializer&&) = delete;
  static_initializer& operator=(static_initializer&) = delete;
  static_initializer& operator=(static_initializer&&) = delete;

private:
  template<class storage_key_t, class U>
  static std::array<hal::byte, sizeof(U)>& get_static_instance_memory()
  {
    static std::array<hal::byte, sizeof(U)> instance_memory{};
    return instance_memory;
  }
};

template<typename storage_key_t, class U>
static std::array<std::uint8_t, sizeof(U)>& get_static_instance_memory()
{
  static std::array<std::uint8_t, sizeof(U)> instance_memory{};
  return instance_memory;
}

template<class T, auto storage_key = [] {}, typename... Args>
result<entity<decltype(storage_key), T>> initialize(Args... args)
{
  // Create a status variable to check if the constructor was successful with
  // setup
  hal::status constructor_status = hal::success();

  // Acquire the statically allocated buffer equal to the size of the object,
  // instance memory, based on the initializer ID and the T.
  auto& instance_memory =
    get_static_instance_memory<decltype(storage_key), T>();

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
}  // namespace hal