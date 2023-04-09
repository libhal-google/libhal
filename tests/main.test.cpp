#include <libhal/config.hpp>

namespace hal {
extern void adc_test();
extern void can_test();
extern void dac_test();
extern void error_test();
extern void i2c_test();
extern void input_pin_test();
extern void interrupt_pin_test();
extern void motor_test();
extern void output_pin_test();
extern void pwm_test();
extern void serial_test();
extern void socket_test();
extern void spi_test();
extern void steady_clock_test();
extern void timeout_test();
extern void timer_test();
extern void servo_test();
extern void g_force_test();
}  // namespace hal

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
  hal::socket_test();
  hal::spi_test();
  hal::steady_clock_test();
  hal::servo_test();
  hal::timeout_test();
  hal::timer_test();
  hal::g_force_test();

  static_assert(
    hal::is_a_test(),
    "The project platform is NOT a test build. Ensure that the "
    "\"libhal.tweaks.hpp\" file exists, can be found in the build system's "
    "include path, and has the platform config option set to \"test\".");
}
