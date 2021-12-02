#pragma once

#include <functional>

namespace embed {
/**
 * @brief General class which will be used to allow for signature to be used and
 * then split by the below class.
 *
 * @tparam owner_class
 * @tparam reference_designator
 * @tparam signature
 */
template<class owner_class, int reference_designator, typename signature>
class static_callable;

/**
 * @brief Specialization of static_callable with the return type and arguments
 * split up.
 *
 * @tparam owner_class see static_callable
 * @tparam reference_designator see static_callable
 * @tparam return_t
 * @tparam args_t
 */
template<class owner_class,
         int reference_designator,
         typename return_t,
         typename... args_t>
class static_callable<owner_class, reference_designator, return_t(args_t...)>
{
public:
  /**
   * @brief Construct a new static callable object
   *
   * @param p_callback
   */
  static_callable(std::function<return_t(args_t...)> p_callback)
  {
    callback = p_callback;
  }

  /**
   * @brief Get the handler object
   *
   * @return auto*
   */
  auto* get_handler() { return &handler; }

private:
  /**
   * @brief private free function that calls the polymorphic callback. This is
   * the function that can be used as entries in an interrupt vector tables or
   * passed as a function pointer to C API.
   *
   * @param args list of arguments
   * @return return_t the return type of the callback
   */
  static return_t handler(args_t... args) { return callback(args...); }

  /**
   * @brief the polymorphic callback to be
   *
   */
  inline static std::function<return_t(args_t...)> callback;
};
} // namespace embed
