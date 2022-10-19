#include <boost/ut.hpp>
#include <libhal/static_list.hpp>
#include <list>

namespace hal {
boost::ut::suite static_list_test = []() {
  using namespace boost::ut;

  "static_list::ctor()"_test = []() {
    // Setup + Exercise + Verify
    [[maybe_unused]] static_list<int> list;
  };

  "static_list::ctor() w/ no_default_constructor"_test = []() {
    struct no_default_constructor
    {
      constexpr no_default_constructor() = delete;
      constexpr explicit no_default_constructor(int p_number)
        : number(p_number)
      {
      }

      int number;
    };
    // Setup
    [[maybe_unused]] static_list<no_default_constructor> list;

    // Exercise + Verify
    auto item = list.push_back(no_default_constructor{ 1 });
  };

  "static_list::push_back()"_test = []() {
    // Setup
    [[maybe_unused]] static_list<int> list;

    // Exercise
    [[maybe_unused]] auto item = list.push_back(5);

    // Verify
    expect(that % 5 == *list.begin());
  };

  "static_list::push_back() & item::get()"_test = []() {
    // Setup
    static_list<int> list;

    // Exercise
    auto item0 = list.push_back(0);
    auto item1 = list.push_back(1);
    auto item2 = list.push_back(2);
    auto item3 = list.push_back(3);

    // Verify
    expect(that % 0 == item0.get());
    expect(that % 1 == item1.get());
    expect(that % 2 == item2.get());
    expect(that % 3 == item3.get());
  };

  "static_list::begin()/end() & static_list::iterator++"_test = []() {
    // Setup
    static_list<int> list;

    // Exercise
    [[maybe_unused]] auto item0 = list.push_back(0);
    [[maybe_unused]] auto item1 = list.push_back(1);
    [[maybe_unused]] auto item2 = list.push_back(2);
    [[maybe_unused]] auto item3 = list.push_back(3);
    [[maybe_unused]] auto item4 = list.push_back(4);

    auto iterator = list.begin();

    // Verify
    // Verify: operator*()
    expect(that % 0 == *iterator);
    // Verify: operator++()
    ++iterator;
    expect(that % 1 == *iterator);
    // Verify: operator*() value should be the same from preincrement
    expect(that % 1 == *iterator);
    // Verify: operator++() + operator*()
    expect(that % 2 == *(++iterator));
    // Verify: operator++(int)
    iterator++;
    expect(that % 3 == *iterator);
    // Verify: operator++(int) + operator*()
    expect(that % 4 == *(++iterator));
    // Verify: ::end == last iterator
    expect(list.end() == ++iterator);
  };

  "static_list::item_iterator::operator++ beyond end"_test = []() {
    // Setup
    static_list<int> list;

    // Exercise
    [[maybe_unused]] auto item0 = list.push_back(0);
    [[maybe_unused]] auto item1 = list.push_back(1);

    auto iterator = list.begin();

    // Verify
    expect(that % 0 == *(iterator++));
    expect(that % 1 == *(iterator++));
    expect(list.end() == iterator++);
    expect(list.end() == iterator++);
    expect(list.end() == iterator++);
    expect(list.end() == iterator++);
  };

  "static_list::item_iterator::operator-- to begin"_test = []() {
    // Setup
    static_list<int> list;

    // Exercise
    [[maybe_unused]] auto item0 = list.push_back(0);
    [[maybe_unused]] auto item1 = list.push_back(1);

    auto iterator = list.end();

    // Verify
    expect(that % 1 == *(--iterator));
    expect(that % 0 == *(--iterator));
    expect(list.begin() == iterator);
  };

  "static_list::item destruct in middle"_test = []() {
    // Setup
    static_list<int> list;

    [[maybe_unused]] auto item0 = list.push_back(0);
    {
      // Exercise
      [[maybe_unused]] auto item1 = list.push_back(1);
    }
    [[maybe_unused]] auto item2 = list.push_back(2);

    auto iterator = list.begin();

    expect(that % 0 == *(iterator++));
    expect(that % 2 == *(iterator++));
    expect(list.end() == iterator);
  };

  "static_list::item destruct head"_test = []() {
    // Setup
    static_list<int> list;

    // Exercise
    {
      [[maybe_unused]] auto item0 = list.push_back(0);
    }
    [[maybe_unused]] auto item1 = list.push_back(1);
    [[maybe_unused]] auto item2 = list.push_back(2);

    auto iterator = list.begin();

    expect(that % 1 == *(iterator++));
    expect(that % 2 == *(iterator++));
    expect(list.end() == iterator);
  };

  "static_list::item destruct tail"_test = []() {
    // Setup
    static_list<int> list;

    [[maybe_unused]] auto item0 = list.push_back(0);
    [[maybe_unused]] auto item1 = list.push_back(1);
    // Exercise
    {
      [[maybe_unused]] auto item2 = list.push_back(2);
    }

    auto iterator = list.begin();

    expect(that % 0 == *(iterator++));
    expect(that % 1 == *(iterator++));
    expect(list.end() == iterator);
  };

  "static_list::item destruct single item"_test = []() {
    // Setup
    static_list<int> list;

    {
      // Exercise
      [[maybe_unused]] auto item0 = list.push_back(0);
    }

    auto iterator = list.begin();

    expect(decltype(iterator)(nullptr) == iterator);
    expect(list.begin() == iterator);
    expect(list.end() == iterator);
    expect(that % 0 == list.size());
    expect(that % true == list.empty());
  };

  "static_list::item move in middle"_test = []() {
    // Setup
    static_list<int> list;
    using iterator_t = decltype(list)::item_iterator;

    [[maybe_unused]] auto item0 = list.push_back(0);
    auto item1 = list.push_back(1);
    [[maybe_unused]] auto item2 = list.push_back(2);

    // Exercise
    auto new_item1 = std::move(item1);

    // Verify
    auto iterator = list.begin();

    expect(that % 0 == *(iterator++));
    expect(iterator != iterator_t(&item1));
    expect(iterator == iterator_t(&new_item1));
    expect(that % 1 == *(iterator++));
    expect(that % 2 == *(iterator++));
    expect(list.end() == iterator);
  };

  "static_list::item move head"_test = []() {
    // Setup
    static_list<int> list;
    using iterator_t = decltype(list)::item_iterator;

    // Exercise
    [[maybe_unused]] auto item0 = list.push_back(0);
    [[maybe_unused]] auto item1 = list.push_back(1);
    [[maybe_unused]] auto item2 = list.push_back(2);
    auto new_item0 = std::move(item0);

    auto iterator = list.begin();

    expect(iterator != iterator_t(&item0));
    expect(iterator == iterator_t(&new_item0));
    expect(that % 0 == *(iterator++));
    expect(that % 1 == *(iterator++));
    expect(that % 2 == *(iterator++));
    expect(list.end() == iterator);
  };

  "static_list::item move tail"_test = []() {
    // Setup
    static_list<int> list;
    using iterator_t = decltype(list)::item_iterator;

    // Exercise
    [[maybe_unused]] auto item0 = list.push_back(0);
    [[maybe_unused]] auto item1 = list.push_back(1);
    [[maybe_unused]] auto item2 = list.push_back(2);
    auto new_item2 = std::move(item2);

    auto iterator = list.begin();

    expect(that % 0 == *(iterator++));
    expect(that % 1 == *(iterator++));
    expect(iterator != iterator_t(&item2));
    expect(iterator == iterator_t(&new_item2));
    expect(that % 2 == *(iterator++));
    expect(list.end() == iterator);
  };

  "static_list::item move single item"_test = []() {
    // Setup
    static_list<int> list;
    using iterator_t = decltype(list)::item_iterator;

    // Exercise
    [[maybe_unused]] auto item = list.push_back(0);
    auto new_item = std::move(item);

    auto iterator = list.begin();

    expect(iterator != iterator_t(&item));
    expect(iterator == iterator_t(&new_item));
    expect(that % 0 == *(iterator++));
    expect(list.end() == iterator);
    expect(that % 1 == list.size());
    expect(that % false == list.empty());
  };

  "static_list mutable ranged for loop"_test = []() {
    // Setup
    static constexpr int offset = 10;
    static_list<int> list;
    int count = 0;

    [[maybe_unused]] auto item0 = list.push_back(0);
    [[maybe_unused]] auto item1 = list.push_back(1);
    [[maybe_unused]] auto item2 = list.push_back(2);
    [[maybe_unused]] auto item3 = list.push_back(3);
    [[maybe_unused]] auto item4 = list.push_back(4);

    {
      // Exercise
      for (auto& item : list) {
        // Verify
        expect(that % count++ == item);
        item += offset;
      }
    }

    count = 0;

    {
      // Exercise
      for (const auto& item : list) {
        // Verify
        expect(that % (count++ + offset) == item);
      }
    }
  };

  "static_list::size()"_test = []() {
    // Setup
    static_list<int> list;

    [[maybe_unused]] auto item0 = list.push_back(0);
    [[maybe_unused]] auto item1 = list.push_back(1);
    [[maybe_unused]] auto item2 = list.push_back(2);
    [[maybe_unused]] auto item3 = list.push_back(3);
    [[maybe_unused]] auto item4 = list.push_back(4);

    expect(that % 5 == list.size());
    expect(that % false == list.empty());
  };

  "static_list::dtor() handles dandling list items"_test = []() {
    // Setup
    auto destroy_list_keep_items = []() -> auto
    {
      static_list<int> list;

      return std::array<static_list<int>::item, 5>{
        list.push_back(), list.push_back(), list.push_back(),
        list.push_back(), list.push_back(),
      };
    };

    // Exercise
    auto array_of_items = destroy_list_keep_items();

    // Verify
    expect(that % nullptr == array_of_items[0].list());
    expect(that % nullptr == array_of_items[1].list());
    expect(that % nullptr == array_of_items[2].list());
    expect(that % nullptr == array_of_items[3].list());
    expect(that % nullptr == array_of_items[4].list());
  };
};
}  // namespace hal
