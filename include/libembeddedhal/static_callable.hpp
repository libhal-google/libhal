#pragma once

#include <functional>

namespace hal {
/**
 * @addtogroup utility
 * @{
 */

/**
 * @brief General class which will be used to allow for signature to be used and
 * then split by the below class.
 *
 * @tparam owner_class class that this static callable belongs to
 * @tparam reference_designator this template argument is used to generate
 * unique static objects for each needed callback
 * @tparam signature function signature to be split up in the static_callable
 * specialization
 */
template<class owner_class, int reference_designator, typename signature>
class static_callable;

/**
 * @brief Specialization of static_callable with the return type and arguments
 * split up.
 *
 * @tparam owner_class see static_callable
 * @tparam reference_designator see static_callable
 * @tparam return_t function's return type
 * @tparam args_t function's set of arguments
 */
template<class owner_class,
         int reference_designator,
         typename return_t,
         typename... args_t>
class static_callable<owner_class,
                      reference_designator,
                      return_t(args_t... p_args)>
{
public:
  /**
   * @brief Construct a new static callable object
   *
   * @param p_callback - when the static callback function is called, it will
   * call this callback
   */
  static_callable(std::function<return_t(args_t... p_args)> p_callback) noexcept
  {
    callback = p_callback;
  }

  /**
   * @brief Get the static function's address
   *
   * @return auto* - static function's address
   */
  [[nodiscard]] auto* get_handler() noexcept
  {
    return &handler;
  }

private:
  /**
   * @brief private free function that calls the polymorphic callback. This is
   * the function that can be used as entries in an interrupt vector tables or
   * passed as a function pointer to C API.
   *
   * @param p_args - list of arguments
   * @return return_t - the return type of the callback
   */
  static return_t handler(args_t... p_args) noexcept
  {
    return callback(p_args...);
  }

  /**
   * @brief the polymorphic callback to be
   *
   */
  inline static std::function<return_t(args_t... p_args)> callback;
};
/** @} */
}  // namespace hal
