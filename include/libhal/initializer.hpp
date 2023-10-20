#pragma once

#include <cstdint>
#include <type_traits>

namespace hal {

template<std::int64_t value>
struct selector_t
{
  static constexpr auto val = value;

  constexpr std::int64_t operator()() const
  {
    return value;
  }
};

template<std::int64_t value>
struct port_t : selector_t<value>
{};

template<std::int64_t value>
struct pin_t : selector_t<value>
{};

template<std::int64_t value>
struct bus_t : selector_t<value>
{};

template<std::int64_t value>
struct channel_t : selector_t<value>
{};

template<std::int64_t value>
struct buffer_t : selector_t<value>
{};

template<typename T>
concept port_param = std::is_same_v<port_t<T::val>, T>;
template<typename T>
concept pin_param = std::is_same_v<pin_t<T::val>, T>;
template<typename T>
concept bus_param = std::is_same_v<bus_t<T::val>, T>;
template<typename T>
concept channel_param = std::is_same_v<channel_t<T::val>, T>;
template<typename T>
concept buffer_param = std::is_same_v<buffer_t<T::val>, T>;

template<std::int64_t value>
inline constexpr port_t<value> port{};
template<std::int64_t value>
inline constexpr pin_t<value> pin{};
template<std::int64_t value>
inline constexpr bus_t<value> bus{};
template<std::int64_t value>
inline constexpr channel_t<value> channel{};
template<std::int64_t value>
inline constexpr buffer_t<value> buffer{};

}  // namespace hal
