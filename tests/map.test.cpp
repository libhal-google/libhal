#include <boost/ut.hpp>
#include <libembeddedhal/map.hpp>

namespace hal {
boost::ut::suite map_test = []() {
  using namespace boost::ut;

  "hal::map()"_test = []() {
    "Zero"_test = []() {
      expect(that % 0 ==
             map(0, { .x = 0, .y = 10 }, { .x = 0, .y = 100 }).value());
    };
    "Boundaries"_test = []() {
      constexpr auto half_positive_value =
        (std::numeric_limits<std::int32_t>::max() / 2) + 1;
      constexpr auto input = std::numeric_limits<std::int32_t>::min() / 2;

      expect(that % half_positive_value ==
             map(input,
                 { .x = std::numeric_limits<std::int32_t>::min(), .y = 0 },
                 { .x = 0, .y = std::numeric_limits<std::int32_t>::max() })
               .value());

      expect(that % 0 == map(0,
                             { .x = std::numeric_limits<std::int32_t>::min(),
                               .y = std::numeric_limits<std::int32_t>::max() },
                             { .x = std::numeric_limits<std::int32_t>::min(),
                               .y = std::numeric_limits<std::int32_t>::max() })
                           .value());

      expect(that % 0 == map(0,
                             { .x = std::numeric_limits<std::int32_t>::min(),
                               .y = std::numeric_limits<std::int32_t>::max() },
                             { .x = 0, .y = 1 })
                           .value());

      expect(that % std::numeric_limits<std::int32_t>::max() ==
             map(1338,
                 { .x = 1337, .y = 1338 },
                 { .x = std::numeric_limits<std::int32_t>::min(),
                   .y = std::numeric_limits<std::int32_t>::max() })
               .value());

      expect(that % std::numeric_limits<std::int32_t>::min() ==
             map(1337,
                 { .x = 1337, .y = 1338 },
                 { .x = std::numeric_limits<std::int32_t>::min(),
                   .y = std::numeric_limits<std::int32_t>::max() })
               .value());
    };
    "Standard usage"_test = []() {
      expect(that % 50 ==
             map(5, { .x = 0, .y = 10 }, { .x = 0, .y = 100 }).value());
      expect(that % 0 ==
             map(5, { .x = 0, .y = 10 }, { .x = -100, .y = 100 }).value());
      expect(that % 50 ==
             map(-5, { .x = -10, .y = 0 }, { .x = 0, .y = 100 }).value());
      expect(that % 25 ==
             map(-75, { .x = -100, .y = 0 }, { .x = 0, .y = 100 }).value());
      expect(that % -175 ==
             map(-75, { .x = -100, .y = 0 }, { .x = -200, .y = -100 }).value());
      expect(that % 10 ==
             map(0, { .x = -10, .y = 10 }, { .x = 10, .y = 10 }).value());
      expect(that % 0 ==
             map(0, { .x = -1, .y = 1 }, { .x = -1, .y = 1 }).value());
    };

    "Exceptions"_test = []() {
      expect(throws([] {
        constexpr auto zero_distance_range =
          map_range<std::int32_t>{ .x = 10, .y = 10 };
        return map(5, zero_distance_range, { .x = 156, .y = 567 }).value();
      }));
    };
  };
};
}
