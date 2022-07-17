#pragma once

#include "bit_limits.hpp"

namespace embed {
/**
 * @addtogroup utility
 * @{
 */

/**
 * @brief Extend a counter's count from an arbitrary bit width to 64-bits by
 * detecting overflows in the count. Each detected overflow is added to an
 * overflow counter which is combined with the current count in order create a
 * count up to 64-bits in length.
 *
 * @tparam CountBitWidth - the bit width of the counter before the count value
 * overflows.
 */
template<size_t CountBitWidth = 32>
class overflow_counter
{
public:
  static_assert(CountBitWidth <= 32, "Bit width cannot exceed 32-bits");
  static_assert(CountBitWidth > 1, "Bit width must be greater than 1");
  /**
   * @brief update the overflow counter, detect if an overflow has occurred, and
   * return the combined
   *
   * @param p_new_count - must be an increasing value and should only decrease
   * when an overflow event occurs.
   * @return constexpr uint64_t - 64-bit count combining the new count value and
   * the overflow count value.
   */
  constexpr uint64_t update(uint32_t p_new_count) noexcept
  {
    // Sanitize the new count value to make sure it does not exceed the
    // designated bit width. Without this check when the count is combined with
    // the overflow value the upper bits may effect the bits of the overflow
    // count, getting an incorrect count value.
    constexpr auto mask = generate_field_of_ones<CountBitWidth, uint32_t>();
    p_new_count = p_new_count & mask;

    // Detect the overflow by checking if the new count is smaller than the
    // previous count. If the count is always increasing, the only way for the
    // new count to be smaller is if the count reached the end of its bit width
    // and overflowed.
    if (m_previous_count > p_new_count) {
      m_overflow_count++;
    }

    // Set previous count to the new count
    m_previous_count = p_new_count;

    // Move overflow count up to the upper bits of the 64-bit number based on
    // CountBitWidth
    uint64_t combined_count = m_overflow_count;
    combined_count <<= CountBitWidth;
    // Add the p_new_count into the combined count to complete it.
    combined_count |= p_new_count;

    return combined_count;
  }

  /**
   * @brief Reset the overflow count back to zero.
   *
   */
  constexpr void reset() noexcept
  {
    m_previous_count = 0;
    m_overflow_count = 0;
  }

private:
  uint32_t m_previous_count = 0;
  uint32_t m_overflow_count = 0;
};
/** @} */
}  // namespace embed
