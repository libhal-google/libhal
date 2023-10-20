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
#include <libhal/initializer.hpp>

#include <boost/ut.hpp>

namespace {
constexpr auto expected_value = float(0.5);
bool return_error_status = false;
bool destructor_called = false;
constexpr int error_int_value = 5;

class test_driver
{
public:
  float m_passed_value{};
  float m_gain{};
  bool m_template_called = false;

  ~test_driver()
  {
    destructor_called = true;
  }

  hal::result<hal::dac::write_t> write(float p_value)
  {
    m_passed_value = p_value * m_gain;
    return hal::dac::write_t{};
  }

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

  test_driver(hal::status& p_status, hal::port_param auto p_port)
    : m_gain(1.0f)
  {
    static_assert(p_port() < 2, "Entity test only supports channels 0 and 1.");
    m_template_called = true;
    HAL_REDIRECT_CHECK(p_status, infallible_call());
    HAL_REDIRECT_CHECK(p_status, fallible_call());
    HAL_REDIRECT_CHECK(p_status, infallible_call());
  }

private:
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

  HAL_ENABLE_INITIALIZER;
  HAL_ENABLE_REINITIALIZER;
};

hal::result<hal::dac*> make_dac(hal::entity_param<test_driver> auto p_entity,
                                hal::channel_param auto p_channel)
{
  class test_driver_dac : public hal::dac
  {
  public:
    test_driver_dac(test_driver* p_driver, std::uint8_t p_channel)
      : m_driver(p_driver)
      , m_channel(p_channel)
    {
    }
    ~test_driver_dac() override = default;

  private:
    hal::result<write_t> driver_write(float p_value) override
    {
      return m_driver->write(p_value * m_channel);
    }

    test_driver* m_driver;
    std::uint8_t m_channel;
  };

  static_assert(0 < p_channel() && p_channel() < 4,
                "This DAC only supports channels 1 to 3");

  static test_driver_dac channel(p_entity.ptr(), p_channel());
  return &channel;
}

class test_singleton
{
public:
  static constexpr bool singleton = true;

  bool m_constructor_called = false;
  int m_counter = 0;

  int increment()
  {
    return m_counter++;
  }

private:
  test_singleton(hal::status&)
  {
    m_constructor_called = true;
  }

  HAL_ENABLE_INITIALIZER;
  HAL_ENABLE_REINITIALIZER;
};

class singleton1 : public test_singleton
{};
class singleton2 : public test_singleton
{};
class singleton3 : public test_singleton
{};

}  // namespace

namespace hal {
void initializer_test()
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

  "hal::initialize<test_driver>() creates driver successfully"_test = []() {
    // Setup
    return_error_status = false;
    auto test_driver_object = initialize<test_driver>().value();
    auto test_subject = make_dac(test_driver_object, hal::channel<2>).value();

    // Exercise
    auto result = test_subject->write(expected_value);

    // Verify
    expect(bool{ result });
    expect(that % (2.0f * expected_value) ==
           test_driver_object->m_passed_value);
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
    auto test_subject = initialize<test_driver>(hal::port<1>).value();
    auto my_pin = initialize<my_pin_driver>(hal::port<1>, hal::pin<15>);
    auto my_serial = initialize<my_uart_driver>(hal::port<3>);

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

  "hal::initialize<test_singleton>() creates singleton successfully"_test =
    []() {
      // Setup
      auto singleton1_a = initialize<singleton1>().value();
      auto singleton1_b = initialize<singleton1>().value();
      auto singleton2_a = initialize<singleton2>().value();

      // Exercise
      // Exercise: Incrementing singleton1_a and singleton1_b should increment
      //           the same variable because they share the same memory.
      auto result0 = singleton1_a->increment();
      auto result1 = singleton1_b->increment();
      auto result2 = singleton2_a->increment();

      // Verify
      expect(that % singleton1_a.ptr() == singleton1_b.ptr());
      expect(that % static_cast<void*>(singleton1_a.ptr()) !=
             static_cast<void*>(singleton2_a.ptr()));
      expect(that % result0 == 0);
      expect(that % result1 == 1);
      expect(that % result2 == 0);
      expect(that % singleton1_a->m_constructor_called);
      expect(that % singleton1_b->m_constructor_called);
      expect(that % singleton2_a->m_constructor_called);
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