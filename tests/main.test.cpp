#include "tests.hpp"

#include <libhal/config.hpp>

int main()
{
  hal::adc_test();
  hal::can_test();
  hal::dac_test();
  hal::error_test();
  hal::i2c_test();
  hal::input_pin_test();
  hal::interrupt_pin_test();
  hal::motor_test();
  hal::output_pin_test();
  hal::pwm_test();
  hal::serial_test();
  hal::servo_test();
  hal::socket_test();
  hal::spi_test();
  hal::steady_clock_test();
  hal::timeout_test();
  hal::timer_test();

  static_assert(
    hal::is_a_test(),
    "The project platform is NOT a test build. Ensure that the "
    "\"libhal.tweaks.hpp\" file exists, can be found in the build system's "
    "include path, and has the platform config option set to \"test\".");
}
