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

class test_entity : public hal::dac
{
public:
  float m_passed_value{};
  float m_gain{};
  bool template_called = false;

  test_entity(test_entity&) = delete;
  test_entity(test_entity&&) = delete;
  test_entity& operator=(test_entity&) = delete;
  test_entity& operator=(test_entity&&) = delete;

  HAL_ENABLE_INITIALIZE;

private:
  test_entity(hal::status& p_status, float p_gain)
    : m_gain(p_gain)
  {
    HAL_REDIRECT_CHECK(p_status, infallible_call());
    [[maybe_unused]] int value = HAL_REDIRECT_CHECK(p_status, fallible_call());
    HAL_REDIRECT_CHECK(p_status, infallible_call());
  }

  test_entity(hal::status& p_status)
    : m_gain(1.0f)
  {
    HAL_REDIRECT_CHECK(p_status, infallible_call());
    [[maybe_unused]] int value = HAL_REDIRECT_CHECK(p_status, fallible_call());
    HAL_REDIRECT_CHECK(p_status, infallible_call());
  }

  template<std::uint64_t channel>
  test_entity(hal::status& p_status, hal::resource_t<channel> p_resource_id)
    : m_gain(1.0f)
  {
    printf(
      "template<%llu> p_resource_id() = %llu\n", channel, p_resource_id()[0]);
    static_assert(p_resource_id()[0] < 2,
                  "Entity test only supports channels 0 and 1.");
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
};
}  // namespace

namespace hal {
void entity_test()
{
  using namespace boost::ut;

  "initialize<test_entity, 0>() successfully"_test = []() {
    // Setup
    return_error_status = false;
    constexpr float gain = 0.5f;
    auto test_subject =
      initialize<test_entity, hal::test_domain(0)>(gain).value();

    // Exercise
    auto result = test_subject->write(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % (expected_value * gain) == test_subject->m_passed_value);
  };

  "initialize<test_entity, 0>() reinitialize 0"_test = []() {
    // Setup
    return_error_status = false;
    constexpr float gain = 0.75f;
    auto test_subject =
      initialize<test_entity, hal::test_domain(0)>(gain).value();

    // Exercise
    auto result = test_subject->write(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % (expected_value * gain) == test_subject->m_passed_value);
  };

  "initialize<test_entity, 0>() failure"_test = []() {
    // Setup
    return_error_status = true;
    auto test_subject = initialize<test_entity, hal::test_domain(0)>(0.75f);

    // Verify
    expect(test_subject.has_error());
  };

  "initialize<test_entity, 1>() successfully"_test = []() {
    // Setup
    return_error_status = false;
    constexpr float gain = 0.5f;
    auto test_subject =
      initialize<test_entity, hal::test_domain(1)>(gain).value();

    // Exercise
    auto result = test_subject->write(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % (expected_value * gain) == test_subject->m_passed_value);
  };

  "initialize<test_entity, 1>() reinitialize 0"_test = []() {
    // Setup
    return_error_status = false;
    constexpr float gain = 0.75f;
    auto test_subject =
      initialize<test_entity, hal::test_domain(1)>(gain).value();

    // Exercise
    auto result = test_subject->write(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % (expected_value * gain) == test_subject->m_passed_value);
  };

  "initialize<test_entity, 1>() failure"_test = []() {
    // Setup
    return_error_status = true;
    auto test_subject = initialize<test_entity, hal::test_domain(1)>(0.75f);

    // Verify
    expect(test_subject.has_error());
  };

  "&initialize<0> != &initialize<1>"_test = []() {
    // Setup
    return_error_status = false;
    auto test0 = initialize<test_entity, hal::test_domain(0)>(0.75f);
    auto test1 = initialize<test_entity, hal::test_domain(1)>(0.45f);

    // Verify
    expect(that % test0.value().ptr() != test1.value().ptr());
  };

  // Test that the constructor used does NOT create a new instance of memory for
  // the driver and confirms that the two memory addresses are the same.
  "&initialize<2>() == &initialize<2>(args)"_test = []() {
    // Setup
    return_error_status = false;
    auto test_2_default = initialize<test_entity, hal::test_domain(2)>();
    auto test_2_with_param =
      initialize<test_entity, hal::test_domain(2)>(0.45f);

    // Verify
    expect(that % test_2_default.value().ptr() ==
           test_2_with_param.value().ptr());
  };

  "Statically evaluate constructor template"_test = []() {
    // Setup
    return_error_status = false;
    auto test_subject =
      initialize<test_entity, hal::test_domain(3)>(hal::resource<1>);

    // Verify
    expect(bool{ test_subject });
    expect(that % true == test_subject.value().ref().template_called);
  };

  "initialize(test_entity, id)"_test = []() {
    // Setup
    return_error_status = false;
    auto test_subject =
      hal::initialize<test_entity, hal::test_domain(3)>(hal::resource<1>);

    // Verify
    expect(bool{ test_subject });
    expect(that % true == test_subject.value().ref().template_called);
  };
};
}  // namespace hal