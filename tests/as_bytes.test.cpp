#include <boost/ut.hpp>
#include <libhal/as_bytes.hpp>

#include <span>
#include <vector>

namespace hal {
boost::ut::suite as_bytes_test = []() {
  using namespace boost::ut;

  "hal::as_bytes()"_test = []() {
    "Zero"_test = []() {
      // Setup
      std::span<const hal::byte> expected_const{};
      std::span<hal::byte> expected_writable{};
      std::vector<float> empty_vector;

      {
        // Exercise
        auto actual = hal::as_bytes(std::span<int>{});
        // Verify
        expect(that % expected_const.data() == actual.data());
        expect(that % expected_const.size() == actual.size());
      }
      {
        // Exercise
        auto actual = hal::as_bytes(empty_vector);
        // Verify
        expect(that % expected_const.data() == actual.data());
        expect(that % expected_const.size() == actual.size());
      }
      {
        // Exercise
        auto actual = hal::as_writable_bytes(expected_writable);
        // Verify
        expect(that % expected_writable.data() == actual.data());
        expect(that % expected_writable.size() == actual.size());
      }
    };

    "One"_test = []() {
      // Setup
      std::array<std::int32_t, 1> array{ 1234 };
      std::vector<std::int32_t> vector;
      vector.push_back(1);

      auto* array_pointer = reinterpret_cast<const hal::byte*>(array.data());
      auto* vector_pointer = reinterpret_cast<const hal::byte*>(vector.data());

      auto vector_byte_size =
        sizeof(decltype(vector)::value_type) * vector.size();
      auto array_byte_size = sizeof(decltype(array)::value_type) * array.size();

      {
        // Exercise
        auto actual = hal::as_bytes(array);
        // Verify
        expect(that % array_pointer == actual.data());
        expect(that % array_byte_size == actual.size());
      }
      {
        // Exercise
        auto actual = hal::as_bytes(vector);
        // Verify
        expect(that % vector_pointer == actual.data());
        expect(that % vector_byte_size == actual.size());
      }
      {
        // Exercise
        auto actual = hal::as_writable_bytes(array);
        // Verify
        expect(that % array_pointer == actual.data());
        expect(that % array_byte_size == actual.size());
      }
    };

    "Standard Usage"_test = []() {
      // Setup
      std::array<std::int32_t, 17> array{ 1234 };
      std::vector<std::int32_t> vector;
      vector.push_back(1);
      vector.push_back(2);
      vector.push_back(3);
      vector.push_back(4);
      vector.push_back(10293);

      auto* array_pointer = reinterpret_cast<const hal::byte*>(array.data());
      auto* vector_pointer = reinterpret_cast<const hal::byte*>(vector.data());

      constexpr auto array_element_size = sizeof(decltype(array)::value_type);
      constexpr auto array_byte_size = array_element_size * array.size();

      constexpr auto vector_element_size = sizeof(decltype(vector)::value_type);
      const auto vector_byte_size = vector_element_size * vector.size();

      {
        // Exercise
        auto actual = hal::as_bytes(array);
        // Verify
        expect(that % array_pointer == actual.data());
        expect(that % array_byte_size == actual.size());
      }
      {
        // Exercise
        auto actual = hal::as_bytes(vector);
        // Verify
        expect(that % vector_pointer == actual.data());
        expect(that % vector_byte_size == actual.size());
      }
      {
        // Exercise
        auto actual = hal::as_writable_bytes(array);
        // Verify
        expect(that % array_pointer == actual.data());
        expect(that % array_byte_size == actual.size());
      }
      {
        // Exercise
        auto actual = hal::as_bytes("Hello World");
        // Verify
        // Verify: this works because almost all compilers will find common
        // strings and place them in the same location, therefor, the location
        // of a string "Hello World" will always be the same.
        expect(that % reinterpret_cast<const hal::byte*>("Hello World") ==
               actual.data());
        expect(that % sizeof("Hello World") == actual.size());
      }
      {
        int c_int_array[] = { 1, 2, 3, 5 };
        // Exercise
        auto actual = hal::as_bytes(c_int_array);
        // Verify
        // Verify: this works because almost all compilers will find common
        // strings and place them in the same location, therefor, the location
        // of a string "Hello World" will always be the same.
        expect(that % reinterpret_cast<const hal::byte*>(c_int_array) ==
               actual.data());
        expect(that % sizeof(c_int_array) == actual.size());
      }
    };
  };
};
}