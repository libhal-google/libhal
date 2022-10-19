#pragma once

#include <functional>

namespace hal::mock {
/**
 * @addtogroup mock
 * @{
 */

/**
 * @brief General class which will be used to allow for signature to be used and
 * then split by the below class.
 *
 * @tparam signature function signature to be split up in the callback_recorder
 * specialization
 */
template<typename signature>
class callback_recorder;

/**
 * @brief Specialization of callback_recorder with the return type and arguments
 * split up.
 *
 * @tparam return_t function's return type
 * @tparam args_t function's set of arguments
 */
template<typename return_t, typename... args_t>
class callback_recorder<return_t(args_t... p_args)>
{
public:
  using callback_function = std::function<return_t(args_t... p_args)>;

  /**
   * @brief Construct a new static callable object
   *
   * @param p_callback - when the static callback function is called, it will
   * call this callback
   */
  template<typename U = return_t>
  callback_recorder(typename std::enable_if_t<std::is_void_v<U>>* = 0) noexcept
  {
    m_callback = [this]([[maybe_unused]] args_t... p_args) { m_call_count++; };
  }

  /**
   * @brief Construct a new static callable object
   *
   * @param p_callback - when the static callback function is called, it will
   * call this callback
   */
  template<typename U = return_t>
  callback_recorder(U p_default = U{},
                    typename std::enable_if_t<!std::is_void_v<U>>* = 0) noexcept
  {
    m_callback = [this, p_default]([[maybe_unused]] args_t... p_args) -> U {
      m_call_count++;
      return p_default;
    };
  }

  /**
   * @brief Construct a new static callable object
   *
   * @param p_callback - when the static callback function is called, it will
   * call this callback
   */
  callback_recorder(callback_function p_callback) noexcept
  {
    m_callback = [this,
                  p_callback]([[maybe_unused]] args_t... p_args) -> return_t {
      m_call_count++;
      return p_callback(p_args...);
    };
  }

  /**
   * @brief Get the static function's address
   *
   * @return auto* - static function's address
   */
  [[nodiscard]] auto& callback() noexcept
  {
    return m_callback;
  }

  /**
   * @brief Returns true if the callback was ever called
   *
   * @return true - was called
   * @return false - has not been called
   */
  [[nodiscard]] bool called_once() noexcept
  {
    return m_call_count > 0;
  }

  /**
   * @brief Get the number of calls to the handler
   *
   * @return auto - number of calls to the callback
   */
  [[nodiscard]] auto call_count() noexcept
  {
    return m_call_count;
  }

  /**
   * @brief Clear call count
   *
   */
  void clear_call_count() noexcept
  {
    m_call_count = 0;
  }

private:
  std::function<return_t(args_t... p_args)> m_callback;
  std::uint32_t m_call_count = 0;
};
/** @} */
}  // namespace hal::mock
