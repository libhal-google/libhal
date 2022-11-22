#pragma once

#include <cctype>
#include <cstdint>
#include <optional>
#include <span>
#include <type_traits>

#include "as_bytes.hpp"
#include "comparison.hpp"
#include "enum.hpp"
#include "error.hpp"
#include "units.hpp"

namespace hal {

/**
 * @brief Concept for a byte
 *
 * @tparam T - object type
 */
template<typename T>
concept byte_stream = requires(T a) {
                        a.state();
                        {
                          a.operator()(std::span<const hal::byte>{})
                          } -> std::same_as<std::span<const hal::byte>>;
                      };

/**
 * @brief Indicate if a byte stream object has finished its work
 *
 * @param p_byte_stream_object - the byte stream object to check
 * @return true - work state is either finished or failed
 * @return false - work state is still in progress
 */
constexpr bool terminated(byte_stream auto p_byte_stream_object)
{
  return terminated(p_byte_stream_object.state());
}

namespace stream {
/**
 * @brief Discard received bytes until the sequence is found
 *
 */
class find
{
public:
  /**
   * @brief Construct a new find object
   *
   * @param p_sequence - sequence to search for. The lifetime of this data
   * pointed to by this span must outlive this object, or not be used when the
   * lifetime of that data is no longer available.
   */
  explicit find(std::span<const hal::byte> p_sequence)
    : m_sequence(p_sequence)
  {
  }

  friend std::span<const hal::byte> operator|(
    const std::span<const hal::byte>& p_input_data,
    find& p_self)
  {
    if (p_input_data.empty()) {
      return p_input_data;
    }

    if (p_self.m_search_index == p_self.m_sequence.size()) {
      return p_input_data;  // forward to next call
    }

    for (size_t index = 0; index < p_input_data.size(); index++) {
      // Check if the next byte received matches the sequence
      if (p_self.m_sequence[p_self.m_search_index] == p_input_data[index]) {
        p_self.m_search_index++;
      } else {  // Otherwise set the search index back to the start.
        p_self.m_search_index = 0;
      }

      // Check if the search index is equal to the size of the sequence size
      if (p_self.m_search_index == p_self.m_sequence.size()) {
        return p_input_data.subspan(index);
      }
    }

    return p_input_data.subspan(p_input_data.size());
  }

  auto state()
  {
    if (m_search_index == m_sequence.size()) {
      return work_state::finished;
    }
    return work_state::in_progress;
  }

private:
  std::span<const hal::byte> m_sequence;
  size_t m_search_index = 0;
};

/**
 * @brief Non-blocking callable for reading serial data into a buffer
 *
 */
class fill
{
public:
  /**
   * @brief Construct a new fill object
   *
   * @param p_buffer - buffer to read data into
   */
  explicit fill(std::span<hal::byte> p_buffer)
    : m_buffer(p_buffer)
  {
  }

  /**
   * @brief Construct a new fill object
   *
   * @param p_buffer - buffer to read data into
   * @param p_fill_amount - reference to a size value to limit the fill amount
   * by.
   */
  fill(std::span<hal::byte> p_buffer, const size_t& p_fill_amount)
    : m_buffer(p_buffer)
    , m_fill_amount(&p_fill_amount)
  {
  }

  friend std::span<const hal::byte> operator|(
    const std::span<const hal::byte>& p_input_data,
    fill& p_self)
  {
    if (p_input_data.empty() || p_self.m_buffer.empty()) {
      return p_input_data;
    }

    auto min_size = std::min(p_input_data.size(), p_self.m_buffer.size());

    if (p_self.m_fill_amount != nullptr) {
      min_size = std::min(*p_self.m_fill_amount, min_size);
    }

    std::copy_n(p_input_data.begin(), min_size, p_self.m_buffer.begin());
    p_self.m_buffer = p_self.m_buffer.subspan(min_size);

    if (p_self.m_fill_amount != nullptr) {
      p_self.m_fill_amount -= min_size;
    }

    return p_input_data.subspan(min_size);
  }

  auto state()
  {
    if (m_buffer.empty()) {
      return work_state::finished;
    }
    return work_state::in_progress;
  }

private:
  std::span<hal::byte> m_buffer;
  const size_t* m_fill_amount = nullptr;
};

/**
 * @brief Discard received bytes until the sequence is found
 *
 */
class fill_upto
{
public:
  /**
   * @brief Construct a new fill upto object
   *
   * @param p_sequence - sequence to search for. The lifetime of this data
   * pointed to by this span must outlive this object, or not be used when the
   * lifetime of that data is no longer available.
   * @param p_buffer - buffer to fill data into
   */
  fill_upto(std::span<const hal::byte> p_sequence,
            std::span<hal::byte> p_buffer)
    : m_sequence(p_sequence)
    , m_buffer(p_buffer)
  {
  }

  friend std::span<const hal::byte> operator|(
    const std::span<const hal::byte>& p_input_data,
    fill_upto& p_self)
  {
    if (p_input_data.empty() ||
        p_self.m_sequence.size() == p_self.m_search_index ||
        p_self.m_buffer.empty()) {
      return p_input_data;
    }

    auto remaining_buffer = p_self.unfilled();
    auto min_size = std::min(p_input_data.size(), remaining_buffer.size());

    for (size_t index = 0; index < min_size; index++) {
      // Check if the search index is equal to the size of the sequence size
      if (p_self.m_search_index == p_self.m_sequence.size()) {
        p_self.m_fill_amount += index;
        return p_input_data.subspan(index);
      }

      // Check if the next byte received matches the sequence
      if (p_self.m_sequence[p_self.m_search_index] == p_input_data[index]) {
        p_self.m_search_index++;
      } else {  // Otherwise set the search index back to the start.
        p_self.m_search_index = 0;
      }

      remaining_buffer[index] = p_input_data[index];
    }

    p_self.m_fill_amount += min_size;
    return p_input_data.subspan(min_size);
  }

  auto state()
  {
    if (m_buffer.empty() && m_search_index != m_sequence.size()) {
      return work_state::failed;
    }
    if (m_search_index == m_sequence.size()) {
      return work_state::finished;
    }
    return work_state::in_progress;
  }

  std::span<hal::byte> span()
  {
    return m_buffer.subspan(0, m_fill_amount);
  }

  std::span<hal::byte> unfilled()
  {
    return m_buffer.subspan(m_fill_amount);
  }

private:
  std::span<const hal::byte> m_sequence;
  std::span<hal::byte> m_buffer;
  size_t m_fill_amount = 0;
  size_t m_search_index = 0;
};

/**
 * @brief Read bytes from stream and convert to integer
 *
 */
template<std::unsigned_integral T>
class parse
{
public:
  /**
   * @brief Construct a new parse object
   */
  explicit parse() = default;

  friend std::span<const hal::byte> operator|(
    const std::span<const hal::byte>& p_input_data,
    parse& p_self)
  {
    if (p_self.m_finished) {
      return p_input_data;
    }

    for (size_t index = 0; index < p_input_data.size(); index++) {
      if (std::isdigit(static_cast<char>(p_input_data[index]))) {
        p_self.m_value *= 10;
        p_self.m_value += p_input_data[index] - hal::byte('0');
        p_self.m_found_digit = true;
      } else if (p_self.m_found_digit) {
        p_self.m_finished = true;
        return p_input_data.subspan(index);
      }
    }

    return p_input_data.last(0);
  }

  work_state state()
  {
    return m_finished ? work_state::finished : work_state::in_progress;
  }

  /**
   * @return T& - return an immutable reference to the value
   */
  const T& value()
  {
    return m_value;
  }

private:
  T m_value = 0;
  bool m_found_digit = false;
  bool m_finished = false;
};

/**
 * @brief Skip number of bytes in a byte stream
 *
 */
class skip
{
public:
  /**
   * @brief Construct a new skip object
   *
   */
  explicit skip(size_t p_skip)
    : m_skip(p_skip)
  {
  }

  friend std::span<const hal::byte> operator|(
    const std::span<const hal::byte>& p_input_data,
    skip& p_self)
  {
    if (p_input_data.empty()) {
      return p_input_data;
    }

    if (p_self.m_skip == 0) {
      return p_input_data;
    }

    auto min = std::min(p_input_data.size(), p_self.m_skip);
    p_self.m_skip -= min;
    return p_input_data.subspan(min);
  }

  work_state state()
  {
    return (m_skip == 0) ? work_state::finished : work_state::in_progress;
  }

private:
  size_t m_skip;
};
}  // namespace stream
}  // namespace hal
