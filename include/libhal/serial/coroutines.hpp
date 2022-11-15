#pragma once

#include <cctype>
#include <cstdint>
#include <optional>
#include <span>

#include "../as_bytes.hpp"
#include "../comparison.hpp"
#include "../enum.hpp"
#include "../error.hpp"
#include "../timeout.hpp"
#include "../units.hpp"
#include "interface.hpp"

namespace hal {
/**
 * @brief Discard received bytes until the sequence is found
 *
 */
class skip_past
{
public:
  /**
   * @brief Construct a new skip beyond object
   *
   * @param p_serial - serial port to skip through
   * @param p_sequence - sequence to search for. The lifetime of this data
   * pointed to by this span must outlive this object, or not be used when the
   * lifetime of that data is no longer available.
   * @param p_read_limit - the maximum number read attempts from the port before
   * returning. A value 0 will result in no reads from the serial port.
   */
  skip_past(serial& p_serial,
            std::span<const hal::byte> p_sequence,
            size_t p_read_limit = 32)
    : m_serial(&p_serial)
    , m_sequence(p_sequence)
    , m_read_limit(p_read_limit)
  {
  }

  /**
   * @brief skip data from the serial port until the sequence is reached.
   *
   * This function will return if the sequence is found or if there are no more
   * bytes in the serial port.
   *
   * Call this function again to resume reading from the port.
   *
   * @return result<work_state> - work_state::in_progress if the sequence hasn't
   * been met and the buffer still has space.
   * @return result<work_state> - work_state::finished if the sequence was
   * found before the buffer was filled completely.
   */
  result<work_state> operator()()
  {
    if (m_search_index == m_sequence.size()) {
      return work_state::finished;
    }

    for (size_t read_limit = 0; read_limit < m_read_limit; read_limit++) {
      std::array<hal::byte, 1> buffer;
      auto read_result = HAL_CHECK(m_serial->read(buffer));

      if (read_result.received.size() != buffer.size()) {
        return work_state::in_progress;
      }

      // Check if the next byte received matches the sequence
      if (m_sequence[m_search_index] == read_result.received[0]) {
        m_search_index++;
      } else {  // Otherwise set the search index back to the start.
        m_search_index = 0;
      }

      // Check if the search index is equal to the size of the sequence size
      if (m_search_index == m_sequence.size()) {
        return work_state::finished;
      }
    }

    return work_state::in_progress;
  }

private:
  serial* m_serial;
  std::span<const hal::byte> m_sequence;
  size_t m_read_limit;
  size_t m_search_index = 0;
};

/**
 * @brief Non-blocking callable for reading serial data into a buffer
 *
 */
class read_into
{
public:
  /**
   * @brief Construct a new read_into object
   *
   * @param p_serial - serial port to skip through
   * @param p_buffer - buffer to read data into
   * @param p_read_limit - the maximum number read attempts from the port before
   * returning. A value 0 will result in no reads from the serial port.
   */
  read_into(serial& p_serial,
            std::span<hal::byte> p_buffer,
            size_t p_read_limit = 32)
    : m_serial(&p_serial)
    , m_buffer(p_buffer)
    , m_read_limit(p_read_limit)
  {
  }

  /**
   * @brief read data into the buffer.
   *
   * This function will return if the read limit is reached or if there are no
   * more bytes in the serial port.
   *
   * Call this function again to resume reading from the port.
   *
   * @return result<work_state> - work_state::in_progress if the sequence hasn't
   * been met and the buffer still has space.
   * @return result<work_state> - work_state::finished if the sequence was
   * found before the buffer was filled completely.
   */
  result<work_state> operator()()
  {
    for (size_t read_limit = 0; read_limit < m_read_limit; read_limit++) {
      if (m_buffer.empty()) {
        return work_state::finished;
      }

      auto read_result = HAL_CHECK(m_serial->read(m_buffer));
      // Set the m_buffer to the amount of bytes remaining to be read.
      m_buffer = read_result.remaining;

      if (read_result.received.empty()) {
        return work_state::in_progress;
      }
    }

    return work_state::in_progress;
  }

private:
  serial* m_serial;
  std::span<hal::byte> m_buffer;
  size_t m_read_limit;
};

/**
 * @brief Discard received bytes until the sequence is found
 *
 */
class read_upto
{
public:
  /**
   * @brief Construct a new skip beyond object
   *
   * @param p_serial - serial port to skip through
   * @param p_sequence - sequence to search for. The lifetime of this data
   * pointed to by this span must outlive this object, or not be used when the
   * lifetime of that data is no longer available.
   * @param p_buffer - buffer to fill data into
   * @param p_read_limit - the maximum number of bytes to read off from the
   * serial port before returning. A value 0 will result in no reads from the
   * serial port.
   */
  read_upto(serial& p_serial,
            std::span<const hal::byte> p_sequence,
            std::span<hal::byte> p_buffer,
            size_t p_read_limit = 32)
    : m_serial(&p_serial)
    , m_sequence(p_sequence)
    , m_buffer(p_buffer)
    , m_read_limit(p_read_limit)
  {
  }

  /**
   * @brief read data into the buffer.
   *
   * This function will return if the read limit is reached or if there are no
   * more bytes in the serial port.
   *
   * Call this function again to resume reading from the port.
   *
   * @return result<work_state> - work_state::in_progress if the sequence hasn't
   * been met and the buffer still has space.
   * @return result<work_state> - work_state::failed if the sequence wasn't
   * found before the buffer was filled completely.
   * @return result<work_state> - work_state::finished if the sequence was
   * found before the buffer was filled completely.
   */
  result<work_state> operator()()
  {
    static constexpr size_t read_length = 1;
    if (m_search_index == m_sequence.size()) {
      return work_state::finished;
    }
    if (m_buffer.empty()) {
      return work_state::failed;
    }

    for (size_t read_limit = 0; read_limit < m_read_limit; read_limit++) {
      auto read_result =
        HAL_CHECK(m_serial->read(m_buffer.subspan(0, read_length)));

      if (read_result.received.size() == 0) {
        return work_state::in_progress;
      }

      // Check if the next byte received matches the sequence
      if (m_sequence[m_search_index] == read_result.received[0]) {
        m_search_index++;
      } else {  // Otherwise set the search index back to the start.
        m_search_index = 0;
      }

      // Check if the search index is equal to the size of the sequence size
      if (m_search_index == m_sequence.size()) {
        return work_state::finished;
      }

      m_buffer = m_buffer.subspan(read_length);

      if (m_buffer.empty()) {
        return work_state::failed;
      }
    }

    return work_state::in_progress;
  }

private:
  serial* m_serial;
  std::span<const hal::byte> m_sequence;
  std::span<hal::byte> m_buffer;
  size_t m_read_limit;
  size_t m_search_index = 0;
};

/**
 * @brief Read bytes from serial port and convert to integer
 *
 */
class read_uint32
{
public:
  /**
   * @brief Construct a new read_uint32 object
   *
   * @param p_serial - serial port to skip through
   * @param p_read_limit - the maximum number read attempts from the port before
   * returning. A value 0 will result in no reads from the serial port.
   */
  read_uint32(serial& p_serial, size_t p_read_limit = 32)
    : m_serial(&p_serial)
    , m_read_limit(p_read_limit)
  {
  }

  /**
   * @brief parse serial data and convert to an integer
   *
   * This function will return if an integer ws found or no more bytes in the
   * serial port.
   *
   * Call this function again to resume reading from the port.
   *
   * @return result<work_state> - work_state::in_progress - if an integer hasn't
   * been found
   * @return result<work_state> - work_state::finished - integer has been found
   * and a non-integer byte has also been found.
   */
  result<work_state> operator()()
  {
    if (m_finished) {
      return work_state::finished;
    }

    for (size_t read_limit = 0; read_limit < m_read_limit; read_limit++) {
      std::array<hal::byte, 1> buffer;
      auto read_result = HAL_CHECK(m_serial->read(buffer));

      if (read_result.received.size() != buffer.size()) {
        return work_state::in_progress;
      }

      if (std::isdigit(static_cast<char>(read_result.received[0]))) {
        m_integer_value *= 10;
        m_integer_value += read_result.received[0] - hal::byte('0');
        m_found_digit = true;
      } else if (m_found_digit) {
        m_finished = true;
        return work_state::finished;
      }
    }

    return work_state::in_progress;
  }

  /**
   * @return std::optional<uint32_t> - integer if the parsing is finished or
   * std::nullopt
   */
  std::optional<uint32_t> get()
  {
    if (!m_finished) {
      return std::nullopt;
    }
    return m_integer_value;
  }

private:
  serial* m_serial;
  size_t m_read_limit;
  std::uint32_t m_integer_value = 0;
  bool m_found_digit = false;
  bool m_finished = false;
};
}  // namespace hal
