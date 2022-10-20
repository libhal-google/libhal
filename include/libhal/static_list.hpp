#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

namespace hal {
/**
 * @brief static_list is a non-owning non-allocating doubly linked list
 * container with O(1) memory utilization.
 *
 * Items in the list cannot be destroyed by
 *
 * @tparam Object - The type of the elements. Object must be "MoveAssignable"
 * and "MoveConstructible".
 */
template<class Object>
class static_list
{
public:
  class item_iterator;

  /**
   * @brief Item/node within the static linked list
   *
   * This object does not allow copies.
   * When moved, the object will determine its new location in memory, and will
   * update the linked list to the new location and invalidate the old location.
   * This ensures that this item's position in memory, if whether returned from
   * a function or relocated in some other way, is always tracked by the list.
   *
   * An item can only be removed from the list if it is no longer accessible
   * (goes out of scope) or is destructed explicitly (DO NOT DO THIS).
   *
   * The return value of push_back MUST be saved to a variable. Failing to do so
   * will result in the object being destructed and removed from the list due to
   * it being inaccessible.
   *
   *     static_list<int> list;
   *     auto item = list.push_back(5);
   *
   */
  class item
  {
  public:
    friend class item_iterator;
    friend class static_list;

    constexpr item(static_list* p_list, const Object& p_object) noexcept
      : m_list(p_list)
      , m_object(p_object)
    {
      setup_self();
    }

    constexpr item(static_list* p_list, Object&& p_object) noexcept
      : m_list(p_list)
      , m_object(std::move(p_object))
    {
      setup_self();
    }

    constexpr item& operator=(item& p_other) = delete;
    constexpr item(item& p_other) = delete;

    constexpr item& operator=(item&& p_other)
    {
      m_object = std::move(p_other.m_object);
      m_list = p_other.m_list;
      m_previous = p_other.m_previous;
      m_next = p_other.m_next;

      if (!m_list) {
        return *this;
      }

      // Reassign head
      if (m_list->m_head == &p_other) {
        m_list->m_head = this;
      } else {
        // If this isn't the head then there MUST be a PREVIOUS node
        // This still works if "m_next" is nullptr
        m_previous->m_next = this;
      }

      if (m_list->m_tail == &p_other) {
        m_list->m_tail = this;
      } else {
        // If this isn't the tail then there MUST be a NEXT node
        // This still works if "m_previous" is nullptr
        m_next->m_previous = this;
      }

      // Mark p_other as invalid such that destructor will exit early
      p_other.m_list = nullptr;

      return *this;
    }

    constexpr item(item&& p_other)
    {
      *this = std::move(p_other);
    }

    auto& get()
    {
      return m_object;
    }

    const auto& get() const
    {
      return m_object;
    }

    auto& operator*()
    {
      return m_object;
    }

    const auto& operator*() const
    {
      return m_object;
    }

    const auto* list() const
    {
      return m_list;
    }

    ~item()
    {
      // Skip re-arranging the list
      // Skip deducting the size of the list
      if (!m_list) {
        return;
      }

      // Reassign head
      if (m_list->m_head == this) {
        m_list->m_head = m_next;
      } else {
        // If this isn't the head then there MUST be a PREVIOUS node
        // This still works if "m_next" is nullptr
        m_previous->m_next = m_next;
      }

      if (m_list->m_tail == this) {
        m_list->m_tail = m_previous;
      } else {
        // If this isn't the tail then there MUST be a NEXT node
        // This still works if "m_previous" is nullptr
        m_next->m_previous = m_previous;
      }

      m_list->m_size--;
    }

  private:
    void setup_self()
    {
      // Make this item the head if head currently doesn't exist
      if (!m_list->m_head) {
        m_list->m_head = this;
      }
      // Make this item the tail
      if (!m_list->m_tail) {
        m_list->m_tail = this;
      } else {
        m_previous = m_list->m_tail;
        m_list->m_tail->m_next = this;
        m_list->m_tail = this;
      }

      m_list->m_size++;
    }

    static_list* m_list = nullptr;
    item* m_previous = nullptr;
    item* m_next = nullptr;
    Object m_object;
  };

  /**
   * @brief Iterator for the static list
   *
   * Implements the C++ named requirement of "LegacyBidirectionalIterator".
   *
   */
  class item_iterator
  {
  public:
    friend class static_list;

    item_iterator(item* p_item, static_list* p_list = nullptr)
      : m_self(p_item)
      , m_list(p_list)
    {
    }

    item_iterator operator++()
    {
      if (m_self) {
        m_self = m_self->m_next;
      }
      return *this;
    }

    item_iterator operator++([[maybe_unused]] int p_other)
    {
      auto old = *this;  // copy old value
      operator++();      // prefix increment
      return old;        // return old value
    }

    item_iterator operator--()
    {
      if (m_self) {
        m_self = m_self->m_previous;
      } else if (m_list) {
        m_self = m_list->m_tail;
      }
      return *this;
    }

    item_iterator operator--([[maybe_unused]] int p_other)
    {
      auto old = *this;  // copy old value
      operator--();      // prefix increment
      return old;        // return old value
    }

    bool operator==(const item_iterator& p_other) const
    {
      return m_self == p_other.m_self;
    }

    Object& operator*()
    {
      return m_self->get();
    }

    const Object& operator*() const
    {
      return m_self->get();
    }

  private:
    item* m_self;
    static_list* m_list;
  };

  using value_type = Object;
  using reference = Object&;
  using const_reference = const Object&;
  using iterator = item_iterator;
  using const_iterator = const item_iterator;
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using pointer = value_type*;
  using const_pointer = const value_type*;

  constexpr static_list()
  {
  }

  constexpr static_list& operator=(static_list& p_other) = delete;
  constexpr static_list(static_list& p_other) = delete;
  constexpr static_list& operator=(static_list&& p_other) = default;
  constexpr static_list(static_list&& p_other) = default;

  /**
   * @brief Add default constructed item to the end of the list
   *
   * Only works for Object's that support default construction.
   *
   * @return constexpr item - item with default constructed value
   */
  [[nodiscard("List item must be saved, otherwise, the value will be discarded "
              "from the list")]] constexpr item
  push_back() noexcept
  {
    return item(this, Object{});
  }

  /**
   * @brief Append item to the end of the static list.
   *
   * @param p_value - value of node
   * @return constexpr item - item with constructed value based on the input
   */
  [[nodiscard("List item must be saved, otherwise, the value will be discarded "
              "from the list")]] constexpr item
  push_back(const Object& p_value) noexcept
  {
    return item(this, p_value);
  }

  /**
   * @brief Append item to the end of the static list.
   *
   * @param p_value - value of node
   * @return constexpr item - item with constructed value based on the input
   */
  [[nodiscard("List item must be saved, otherwise, the value will be discarded "
              "from the list")]] constexpr item
  push_back(Object&& p_value) noexcept
  {
    return item(this, p_value);
  }

  constexpr bool empty() noexcept
  {
    return m_size == 0;
  }

  constexpr auto begin() noexcept
  {
    return item_iterator(m_head);
  }

  constexpr const auto begin() const noexcept
  {
    return item_iterator(m_head);
  }

  constexpr const auto cbegin() const noexcept
  {
    return item_iterator(m_head);
  }

  constexpr auto end() noexcept
  {
    return item_iterator(nullptr, this);
  }

  constexpr const auto end() const noexcept
  {
    return item_iterator(nullptr, this);
  }

  constexpr const auto cend() const noexcept
  {
    return item_iterator(nullptr, this);
  }

  constexpr std::size_t size() const noexcept
  {
    return m_size;
  }

  ~static_list() noexcept
  {
    if (!m_head || !m_tail) {
      return;
    }

    for (auto start = begin(); start != end(); start++) {
      start.m_self->m_list = nullptr;
    }
  }

private:
  item* m_head = nullptr;
  item* m_tail = nullptr;
  std::size_t m_size = 0;
};
}  // namespace hal
