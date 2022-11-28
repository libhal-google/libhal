#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace hal::bit {

/**
 * @addtogroup utility
 * Available utilities for libhal
 * @{
 */
struct mask
{
  std::uint32_t position;
  std::uint32_t width;

  template<std::uint32_t position1, std::uint32_t position2>
  static consteval mask from()
  {
    constexpr std::uint32_t plus_one = 1;
    if constexpr (position1 < position2) {
      return mask{ .position = position1,
                   .width = plus_one + (position2 - position1) };
    } else {
      return mask{ .position = position2,
                   .width = plus_one + (position1 - position2) };
    }
  }

  template<std::uint32_t position>
  static constexpr mask from()
  {
    return mask{ .position = position, .width = 1U };
  }

  template<std::unsigned_integral T>
  constexpr auto origin_mask() const
  {
    // At compile time, generate variable containing all 1s with the size of the
    // target parameter.
    constexpr T field_of_ones = std::numeric_limits<T>::max();

    // At compile time calculate the number of bits in the target parameter.
    constexpr size_t target_width = sizeof(T) * 8;

    // Create mask by shifting the set of 1s down so that the number of 1s from
    // bit position 0 is equal to the width parameter.
    T mask_at_origin = static_cast<T>(field_of_ones >> (target_width - width));

    return mask_at_origin;
  }

  template<std::unsigned_integral T>
  constexpr auto mask_value() const
  {
    return static_cast<T>(origin_mask<T>() << position);
  }

  constexpr bool operator==(const mask& other)
  {
    return other.position == position && other.width == width;
  }
};

template<mask field>
auto extract(std::unsigned_integral auto p_value)
{
  using T = decltype(p_value);
  // Shift desired value to the right to position 0
  const auto shifted = p_value >> field.position;
  // Mask away any bits left of the value based on the field width
  const auto masked = shifted & field.origin_mask<T>();
  // Leaving only the desired bits
  return static_cast<T>(masked);
}

template<std::unsigned_integral T>
class value
{
public:
  static constexpr std::uint32_t width = sizeof(T) * 8;

  value(T p_initial_value = 0)
    : m_value(p_initial_value)
  {
  }

  template<mask field>
  auto& set()
  {
    static_assert(field.position < width,
                  "Bit position exceeds register width");
    const auto mask = static_cast<T>(1U << field.position);

    m_value = m_value | mask;

    return *this;
  }

  template<mask field>
  auto& clear()
  {
    static_assert(field.position < width,
                  "Bit position exceeds register width");
    const auto mask = static_cast<T>(1U << field.position);
    const auto inverted_mask = ~mask;

    m_value = m_value & inverted_mask;

    return *this;
  }

  template<mask field>
  auto& toggle()
  {
    static_assert(field.position < width,
                  "Bit position exceeds register width");

    const auto mask = static_cast<T>(1U << field.position);

    m_value = m_value ^ mask;

    return *this;
  }

  template<mask field>
  constexpr auto& insert(std::unsigned_integral auto p_value)
  {
    // Clear width's number of bits in the target value at the bit position
    // specified.
    m_value &= ~field.mask_value<T>();

    // AND value with mask to remove any bits beyond the specified width.
    // Shift masked value into bit position and OR with target value.
    auto shifted_field = static_cast<T>(p_value) << field.position;
    auto value = shifted_field & field.mask_value<T>();

    m_value = m_value | static_cast<T>(value);

    return *this;
  }

  template<std::integral U>
  [[nodiscard]] constexpr auto to()
  {
    return static_cast<U>(m_value);
  }

  [[nodiscard]] constexpr T get()
  {
    return m_value;
  }

protected:
  T m_value;
};

template<std::unsigned_integral T>
class modify : public value<T>
{
public:
  constexpr modify(volatile T& p_register_reference)
    : value<T>(p_register_reference)
    , m_pointer(&p_register_reference)
  {
  }

  ~modify()
  {
    *m_pointer = this->m_value;
  }

private:
  volatile T* m_pointer;
};
/** @} */
}  // namespace hal::bit
