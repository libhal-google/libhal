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
#include <libhal/experimental/entity.hpp>

#include <boost/ut.hpp>

namespace {
constexpr auto expected_value = float(0.5);
bool return_error_status = false;

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

class test_driver
  : public hal::dac
  , public hal::static_initializer<test_driver>
{
public:
  float m_passed_value{};
  float m_gain{};
  bool template_called = false;

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
  test_driver(hal::status& p_status, resource_t<channel> p_resource_id)
    : m_gain(1.0f)
  {
    printf(
      "template<%llu> p_resource_id() = %llu\n", channel, p_resource_id()[0]);
    static_assert(channel < 2, "Entity test only supports channels 0 and 1.");
    template_called = true;
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
      return hal::new_error(5);
    }
    return 7;
  }

  hal::result<write_t> driver_write(float p_value) override
  {
    m_passed_value = p_value * m_gain;
    return write_t{};
  }

  friend class hal::static_initializer<test_driver>;
};
}  // namespace

namespace hal {
void entity_test()
{
  using namespace boost::ut;

  "[initializer] test_driver::initialize() successfully"_test = []() {
    // Setup
    return_error_status = false;
    constexpr float gain = 0.5f;
    auto test_subject = test_driver::initialize(gain).value();

    // Exercise
    auto result = test_subject->write(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % (expected_value * gain) == test_subject->m_passed_value);
  };

  "[initializer] Statically evaluate constructor template"_test = []() {
    // Setup
    return_error_status = false;
    auto test_subject = test_driver::initialize(resource<1>);

    // Verify
    expect(bool{ test_subject });
    expect(that % true == test_subject.value().ref().template_called);
  };

  "[initializer] test_driver::initialize() reinitialize 0"_test = []() {
    // Setup
    return_error_status = false;
    constexpr float gain = 0.75f;
    auto test_subject = test_driver::initialize(gain).value();

    // Exercise
    auto result = test_subject->write(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % (expected_value * gain) == test_subject->m_passed_value);
  };

  "[initializer] test_driver::initialize() failure"_test = []() {
    // Setup
    return_error_status = true;
    auto test_subject = test_driver::initialize(0.75f);

    // Verify
    expect(test_subject.has_error());
  };

  "[initializer] test_driver::initialize() successfully"_test = []() {
    // Setup
    return_error_status = false;
    constexpr float gain = 0.5f;
    auto test_subject = test_driver::initialize(gain).value();

    // Exercise
    auto result = test_subject->write(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % (expected_value * gain) == test_subject->m_passed_value);
  };

  "[initializer] test_driver::initialize() reinitialize 0"_test = []() {
    // Setup
    return_error_status = false;
    constexpr float gain = 0.75f;
    auto test_subject = test_driver::initialize(gain).value();

    // Exercise
    auto result = test_subject->write(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % (expected_value * gain) == test_subject->m_passed_value);
  };

  "[initializer] test_driver::initialize() failure"_test = []() {
    // Setup
    return_error_status = true;
    auto test_subject = test_driver::initialize(0.75f);

    // Verify
    expect(test_subject.has_error());
  };

  "[initializer] &initialize != &initialize"_test = []() {
    // Setup
    return_error_status = false;
    auto test0 = test_driver::initialize(0.75f);
    auto test1 = test_driver::initialize(0.45f);

    // Verify
    expect(that % test0.value().ptr() != test1.value().ptr());
  };
};
}  // namespace hal