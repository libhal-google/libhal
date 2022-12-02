#pragma once

#include <span>

#include "units.hpp"

namespace hal {
template<typename T>
constexpr std::span<hal::byte> as_writable_bytes(T* p_address,
                                                 size_t p_number_of_elements)
{
  auto* start = reinterpret_cast<hal::byte*>(p_address);
  size_t number_of_bytes = sizeof(T) * p_number_of_elements;
  return std::span<hal::byte>(start, number_of_bytes);
}

template<typename T>
constexpr std::span<const hal::byte> as_bytes(const T* p_address,
                                              size_t p_number_of_elements)
{
  auto* start = reinterpret_cast<const hal::byte*>(p_address);
  size_t number_of_bytes = sizeof(T) * p_number_of_elements;
  return std::span<const hal::byte>(start, number_of_bytes);
}

template<typename T>
concept convertible_to_bytes = requires(T a) {
                                 *a.data();
                                 a.size();
                               };

constexpr std::span<hal::byte> as_writable_bytes(
  convertible_to_bytes auto& p_container)
{
  return as_writable_bytes(p_container.data(), p_container.size());
}

constexpr std::span<const hal::byte> as_bytes(
  const convertible_to_bytes auto& p_container)
{
  return as_bytes(p_container.data(), p_container.size());
}
}  // namespace hal
