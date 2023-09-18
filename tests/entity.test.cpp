// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <libhal/dac.hpp>
#include <libhal/entity.hpp>

#include <boost/ut.hpp>

namespace {
constexpr auto expected_value = float(0.5);
bool return_error_status = false;
bool destructor_called = false;
constexpr int error_int_value = 5;

template<std::uint64_t... identifier>
struct resource_t
{
  // static constexpr auto value = identifier;

  static constexpr std::array<uint64_t, sizeof...(identifier)> value{
    identifier...
  };

  constexpr auto operator()()
  {
    return value;
  }
};

template<std::uint64_t identifier>
inline constexpr auto resource = resource_t<identifier>{};

struct test_driver : public hal::dac
{
  float m_passed_value{};
  float m_gain{};
  bool m_template_called = false;

  ~test_driver() override
  {
    destructor_called = true;
  }

private:
  test_driver(hal::status& p_status, float p_gain)
    : m_gain(p_gain)
  {
    HAL_REDIRECT_CHECK(p_status, infallible_call());
    [[maybe_unused]] int value = HAL_REDIRECT_CHECK(p_status, fallible_call());
    HAL_REDIRECT_CHECK(p_status, infallible_call());
  }

  test_driver(hal::status& p_status)
    : m_gain(1.0f)
  {
    HAL_REDIRECT_CHECK(p_status, infallible_call());
    [[maybe_unused]] int value = HAL_REDIRECT_CHECK(p_status, fallible_call());
    HAL_REDIRECT_CHECK(p_status, infallible_call());
  }

  template<std::uint64_t channel>
  test_driver(hal::status& p_status,
              [[maybe_unused]] resource_t<channel> p_resource_id)
    : m_gain(1.0f)
  {
    static_assert(channel < 2, "Entity test only supports channels 0 and 1.");
    m_template_called = true;
    HAL_REDIRECT_CHECK(p_status, infallible_call());
    HAL_REDIRECT_CHECK(p_status, fallible_call());
    HAL_REDIRECT_CHECK(p_status, infallible_call());
  }

  hal::status infallible_call()
  {
    return hal::success();
  }

  hal::result<int> fallible_call()
  {
    if (return_error_status) {
      return hal::new_error(error_int_value);
    }
    return 7;
  }

  hal::result<write_t> driver_write(float p_value) override
  {
    m_passed_value = p_value * m_gain;
    return write_t{};
  }

  HAL_ENABLE_INITIALIZER;
  HAL_ENABLE_REINITIALIZER;
};
}  // namespace

namespace hal {
void entity_test()
{
  using namespace boost::ut;

  "hal::initialize<test_driver>() creates driver successfully"_test = []() {
    // Setup
    return_error_status = false;
    auto test_subject = initialize<test_driver>().value();

    // Exercise
    auto result = test_subject->write(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % expected_value == test_subject->m_passed_value);
    expect(that % !test_subject.ref().m_template_called);
  };

  "hal::initialize<test_driver>(): call constructor w/ gain"_test = []() {
    // Setup
    return_error_status = false;
    constexpr float gain = 0.5f;
    auto test_subject = initialize<test_driver>(gain).value();

    // Exercise
    auto result = test_subject->write(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % (expected_value * gain) == test_subject->m_passed_value);
    expect(that % !test_subject.ref().m_template_called);
  };

  "hal::initialize<test_driver>(): call template constructor"_test = []() {
    // Setup
    return_error_status = false;
    auto test_subject = initialize<test_driver>(resource<1>).value();

    // Verify
    expect(that % test_subject.ref().m_template_called);
  };

  "hal::initialize: each call generates new statically allocated objects"_test =
    []() {
      // Setup
      return_error_status = false;
      auto test0 = initialize<test_driver>(0.75f);
      auto test1 = initialize<test_driver>(0.45f);

      // Verify
      expect(that % test0.value().ptr() != test1.value().ptr());
      expect(that % test0.value()->m_gain != test1.value()->m_gain);
    };

  "initialize<test_driver>(): report constructor failure"_test = []() {
    // Setup
    return_error_status = true;
    hal::attempt_all(
      []() -> hal::status {
        [[maybe_unused]] auto test_subject =
          HAL_CHECK(initialize<test_driver>(0.75f));

        return hal::success();
      },
      // Verify
      [](int p_error_number) {
        expect(that % error_int_value == p_error_number);
      },
      []() { expect(false) << "(int) handler not handled!"; });
  };

  "reinitialize(test_subject) updates gain"_test = []() {
    // Setup
    return_error_status = false;
    constexpr float original_gain = 0.75f;
    constexpr float new_gain = 0.32f;
    auto test_subject = initialize<test_driver>(original_gain).value();
    expect(that % original_gain == test_subject->m_gain);

    // Exercise
    auto reinit_status = reinitialize(test_subject, new_gain);

    // Verify
    expect(that % bool{ reinit_status });
    expect(that % new_gain == test_subject->m_gain);
  };

  "reinitialize(test_subject) reinitialize fails"_test = []() {
    // Setup
    return_error_status = false;
    destructor_called = false;
    constexpr float original_gain = 0.01f;
    constexpr float new_gain = 0.97f;
    auto test_subject = initialize<test_driver>(original_gain).value();
    expect(that % original_gain == test_subject->m_gain);

    // Exercise
    return_error_status = true;
    hal::attempt_all(
      [&test_subject]() -> hal::status {
        return reinitialize(test_subject, new_gain);
      },
      // Verify
      [](int p_error_number) {
        expect(that % error_int_value == p_error_number);
      },
      []() { expect(false) << "(int) handler not handled!"; });
    expect(that % destructor_called) << "destructor was not called!";
  };
};
}  // namespace hal