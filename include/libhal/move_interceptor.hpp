#pragma once

namespace hal {
/**
 * @addtogroup utility
 * @{
 */
/**
 * @brief Use this to perform changes on an object its move constructor is
 * executed.
 *
 * This can be used to allow type T to use a default move constructor but allow
 * specific operations to occur beforehand. This is used for moveable objects
 * with callbacks that refer back to their object's address such as the
 * following:
 *
 *     this->obj->set_callback([this]() { foo(); });
 *
 * The default move constructor does everything correctly for type T. But the
 * address of "this" in the lambda expression refers to the previous object's
 * address which is no longer valid. Rather than a make a whole move constructor
 * just to update this one callback, the move_interceptor can be used to change
 * the previous object's callback to the new object's address before the move
 * occurs. Then the object's default move constructor is executed.
 *
 * @tparam T - object to intercept the move constructor of
 */
template<class T>
struct move_interceptor
{
  /**
   * @brief Function called prior to type T's move constructor
   *
   * T must have a function named `T::intercept(T* p_previous)`.
   *
   * @param p_previous - pointer to the previous object that will be moved to
   * the new object.
   */
  void intercept(T* p_previous)
  {
    static_cast<T*>(this)->intercept(p_previous);
  }

  move_interceptor() = default;
  move_interceptor(move_interceptor& p_previous) = delete;
  move_interceptor& operator=(move_interceptor& p_previous) = delete;
  move_interceptor(move_interceptor&& p_previous)
  {
    intercept(static_cast<T*>(&p_previous));
  }
  move_interceptor& operator=(move_interceptor&& p_previous)
  {
    intercept(static_cast<T*>(&p_previous));
    return *this;
  }
};
/** @} */
}  // namespace hal
