#pragma once

#include <functional>

namespace embed {
// General class which will be used to allow for signature to be used and then
// split by the below class.
template<class owner_class, int reference_designator, typename signature>
class static_callable;

// Specialization of the above class but with the return type and arguments
// split up
template<class owner_class,
         int reference_designator,
         typename return_t,
         typename... args_t>
class static_callable<owner_class, reference_designator, return_t(args_t...)>
{
public:
  static_callable(std::function<return_t(args_t...)> p_callback)
  {
    callback = p_callback;
  }

  auto* get_handler() { return &handler; }

private:
  static return_t handler(args_t... args)
  {
    return callback(args...);
  }

  inline static std::function<return_t(args_t...)> callback;
};
} // namespace embed
