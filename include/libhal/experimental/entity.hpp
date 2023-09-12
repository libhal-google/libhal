#pragma once

#include <array>
#include <cstdint>

#include "../error.hpp"
#include "../units.hpp"

namespace hal {

constexpr std::uint64_t user_domain(std::uint64_t p_unique_id)
{
  return p_unique_id + (0ULL << 32ULL);
}

constexpr std::uint64_t test_domain(std::uint64_t p_unique_id)
{
  return p_unique_id + (1ULL << 32ULL);
}

constexpr std::uint64_t application_domain(std::uint64_t p_unique_id)
{
  return p_unique_id + (2ULL << 32ULL);
}

template<std::uint64_t id, class T, class... U>
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

template<std::uint64_t... identifier>
struct resource_t
{
  // static constexpr auto value = identifier;

  static constexpr std::array<uint64_t, sizeof...(identifier)> value{
    identifier...
  };

  constexpr auto operator()()
  {
    return value;
  }
};

template<std::uint64_t identifier>
inline constexpr auto resource = resource_t<identifier>{};

template<class T>
class static_initializer
{
public:
  template<std::uint64_t id, typename... Args>
  static result<entity<id, T>> initialize(Args... args)
  {
    // Create a status variable to check if the constructor was successful with
    // setup
    hal::status constructor_status = hal::success();

    // Acquire the statically allocated buffer equal to the size of the object,
    // instance memory, based on the initializer ID and the T.
    auto& instance_memory = get_static_instance_memory<id, T>();

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

private:
  template<std::uint64_t id, class U>
  static std::array<hal::byte, sizeof(U)>& get_static_instance_memory()
  {
    static std::array<hal::byte, sizeof(U)> instance_memory{};
    return instance_memory;
  }
};

template<std::uint64_t id, class U>
static std::array<hal::byte, sizeof(U)>& get_static_instance_memory()
{
  static std::array<hal::byte, sizeof(U)> instance_memory{};
  return instance_memory;
}

template<class T, std::uint64_t id, typename... Args>
result<entity<id, T>> initialize(Args... args)
{
  // Create a status variable to check if the constructor was successful with
  // setup
  hal::status constructor_status = hal::success();

  // Acquire the statically allocated buffer equal to the size of the object,
  // instance memory, based on the initializer ID and the T.
  auto& instance_memory = get_static_instance_memory<id, T>();

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

#define HAL_INIT_CHECK(__status_pointer, __expression)                         \
  ({                                                                           \
    auto&& BOOST_LEAF_TMP = (__expression);                                    \
    if (!BOOST_LEAF_TMP) {                                                     \
      __status_pointer = BOOST_LEAF_TMP;                                       \
      return;                                                                  \
    }                                                                          \
    std::move(BOOST_LEAF_TMP);                                                 \
  }).value()

#define HAL_ENABLE_INITIALIZE                                                  \
  template<class T, std::uint64_t id, typename... Args>                        \
  friend hal::result<hal::entity<id, T>> hal::initialize(Args... args)
