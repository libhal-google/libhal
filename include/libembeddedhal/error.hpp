#pragma once

#include <array>
#include <span>

#if __has_include(<source_location>)
#include <source_location>
#else
#include <experimental/source_location>
namespace std {
using namespace std::experimental;
}
#endif

#include "config.hpp"
#include "internal/third_party/leaf.hpp"

namespace embed::error {
/**
 * @brief Used for defining static_asserts that should always fail, but only if
 * the static_assert line is hit via `if constexpr` control block. Prefer to NOT
 * use this directly but to use `invalid_option` instead
 *
 * @tparam options ignored by the application but needed to create a non-trivial
 * specialization of this class which allows its usage in static_assert.
 */
template<auto... options>
struct invalid_option_t : std::false_type
{};
/**
 * @brief Helper definition to simplify the usage of invalid_option_t.
 *
 * @tparam options ignored by the application but needed to create a non-trivial
 * specialization of this class which allows its usage in static_assert.
 */
template<auto... options>
inline constexpr bool invalid_option = invalid_option_t<options...>::value;

/**
 * @brief An association error type for all libembeddedhal drivers that inherit
 * from this class. It is used to disambiguate errors coming from a
 * libembeddedhal embed::driver vs those coming from other libraries.
 *
 */
struct universal
{};

/**
 * @brief An error indicating a timeout event.
 *
 * Timeouts occur when a particular operation is given an amount of time to
 * complete. If the operation cannot be completed in that time, this object
 * should be thrown.
 *
 */
struct timeout
{};

/**
 * @brief Error type indicating that the settings for serial uart could not be
 * set
 *
 * <b>How to handle these errors:</b>
 *
 * - The nature of this error signifies an bug in the code. The documentation
 *   of the peripheral and the API of the driver should be consulted in order
 *   to determine what are possible settings for this serial port.
 *
 */
struct invalid_settings
{};

/**
 * @brief A structure to hold stack strace source location entries. The size of
 * this class can be changed by changing the
 * embed::config::stacktrace_depth_limit value in the libembeddedhal.tweak.hpp
 * file for your project. This can be used to reduce the stack size of functions
 * that have Boost.LEAF handlers that inspect a embed::error::stacktrace.
 *
 */
class stacktrace
{
public:
  /**
   * @brief Get the stack trace list items.
   *
   * @return auto - return a const span with length equal to the number of
   * entries in the stacktrace list.
   */
  auto get() const noexcept
  {
    return std::span<const boost::leaf::e_source_location>(m_list.begin(),
                                                           m_count);
  }

  /**
   * @brief Add a source location to the stack trace list
   *
   * @param p_source_location - source location to be added to the list
   */
  void append(boost::leaf::e_source_location p_source_location) noexcept
  {
    if constexpr (config::get_stacktrace_on_error) {
      if (m_count < m_list.size()) {
        m_list[m_count] = std::move(p_source_location);
        m_count++;
      }
    }
  }

private:
  std::array<boost::leaf::e_source_location, config::stacktrace_depth_limit>
    m_list{};
  size_t m_count = 0;
};

/**
 * @brief Every function within libembeddedhal and implementing its interfaces
 * that returns a boost::leaf::result<T> and can return an error must have the
 * following line at the start of its function definition:
 *
 *     auto on_error = embed::error::setup();
 *
 * If an error occurs, this function ensures the following:
 *
 * - That embed::error::universal is associated with the error which is
 *   necessary to allow handlers to capture any libembeddedhal specific error
 *   but ignore the actual error itself. This is similar to a
 *   `catch(std::exception const&)` where you simply want to capture any and all
 *   exceptions deriving from std::exception but still allow other exceptions to
 *   pass through.
 * - That a stacktrace is captured for the path where an error occurred
 *
 * Since this function setups handler for when errors occur, it does not make
 * sense to call this function for functions that return boost::leaf::result<T>
 * but can never return an error. Thus this function should not be called on
 * such functions.
 *
 * @param p_function_name - the name of the calling function
 * @param p_file_name - the name of the file in which this function was called
 * @param p_line_number - the line number in which this function was called
 * @return auto - error handler for when an error is emitted on the calling
 * functions frame.
 */
inline auto setup(
  const char* p_function_name = std::source_location::current().function_name(),
  const char* p_file_name = std::source_location::current().file_name(),
  int p_line_number = std::source_location::current().line()) noexcept
{
  // Default strings for file and function that will be reassigned to the actual
  // strings, if their corresponding config options are set to true.
  const char* file = "";
  const char* function = "";
  int line = -1;

  // The two following `if constexpr` will ensure that the compiler optimizes
  // away the function and filename strings if their corresponding configuration
  // values are set to false. This will eliminate many of the function strings
  // associated with the "error" or "sad" path of a project, saving flash space.
  if constexpr (embed::config::get_stacktrace_on_error) {
    function = p_function_name;
  }

  if constexpr (embed::config::get_source_position_on_error) {
    file = p_file_name;
    line = p_line_number;
  }

  // Add the embed::error::universal type to the set of active errors. This is
  // Required for any library implementing an interface for libembeddedhal.
  // This allows catching of any and all error from libembeddedhal.
  //
  // In order to generate a stacktrace for functions that can possibly produce
  // errors, this on_error handler captures the function name of the function
  // that called this function. When an error occurs, that function name will be
  // appended to the end of the stacktrace's list of functions.
  return boost::leaf::on_error(universal{},
                               [file, function, line](stacktrace& p_trace) {
                                 p_trace.append(boost::leaf::e_source_location{
                                   .file = file,
                                   .line = line,
                                   .function = function,
                                 });
                               });
}
}  // namespace embed::error

/// shorthand for BOOST_LEAF_CHECK
#define EMBED_CHECK(expr) BOOST_LEAF_CHECK(expr)
