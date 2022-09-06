<p align="center">
  <img height="150" src="logo.svg">
</p>

# libhal

[![Discord](https://img.shields.io/discord/800515757871726622?color=7389D8&logo=discord&logoColor=ffffff&labelColor=6A7EC2)](https://discord.gg/p5A6vzv8tm)
[![docs](https://github.com/libhal/libhal/actions/workflows/docs.yml/badge.svg?branch=main)](https://github.com/libhal/libhal/actions/workflows/docs.yml)
[![lint](https://github.com/libhal/libhal/actions/workflows/lint.yml/badge.svg?branch=main)](https://github.com/libhal/libhal/actions/workflows/lint.yml)
[![tests](https://github.com/libhal/libhal/actions/workflows/tests.yml/badge.svg?branch=main)](https://github.com/libhal/libhal/actions/workflows/tests.yml)

**🏗️ WARNING: PROJECT IN DEVELOPMENT 🚧**

| [Install](#-install)
| [Overview](#ℹ%EF%B8%8F-overview)
| [Glossary](#-glossary)
| [Usage](#%EF%B8%8F-usage)
| [Badges](#-library-badges)
| [Standards](#-library-standards)
| [Libraries](#-libraries)
| [Motivation](#-motivation)
| [Contributing](#-contributing)

# [📚 Software APIs](https://libhal.github.io/libhal/api)

# 📥 Install

## [Installing Prerequisites](docs/prerequisites.md)

## Install using conan center index (CCI)

NOT RECOMMENDED! CCI tends to be out of date currently.

```bash
conan install libhal
```

## Install from `libhal-trunk`

`libhal-trunk` is a remote conan server with the latest version of the code.

Run the following to add `libhal-trunk` to your list of conan remote servers.

NOTE: that the `--insert` argument places this server at the highest priority
for conan, meaning updates will be checked at this server first before
attempting to check out servers like the CCI.

```bash
conan remote add libhal-trunk https://libhal.jfrog.io/artifactory/api/conan/trunk-conan --insert
```

Now run this command to install libhal from that server.

```bash
conan install libhal
```

## Install from source

```bash
git clone https://github.com/libhal/libhal.git
cd libhal
conan create .
```

# ℹ️ Overview

libhal exists to make hardware drivers:

- **🚚 portable**
- **🦾 flexible**
- **📦 accessible**
- **🍰 easy to use**

## Project Attributes

- Header only
- Available on Conan (coming soon to vcpkg)
- Does not throw exceptions
- Does not dynamically allocate
- Uses tweak header files for customization
- Designed to be modular, dynamic, composable, and lightweight
- System agnostic
- Follows C++ Core Guidelines
- Dependencies:
  - C++20
  - [Boost.LEAF](https://boostorg.github.io/leaf/) for error handling

# 📃 Glossary

Here is a list of terms used in libhal. It is HIGHLY RECOMMENDED that
new users of libhal read this section.

## Target

Targets are defined as MCUs (micro-controllers), SOCs (system-on-chip),
operating systems, or operating systems running on a particular SBC
(single-board-computer).

### MCU target examples

- LPC40xx series family of MCUs
- STM32F10x series family of MCUs
- RP2040

### SOCs target examples

- AM335x
- Samsung Exynos5422

### Operating Systems target examples

- Linux
- Windows CE

### SBC example

- Raspberry Pi
- ODROID UX
- BeagleBone Black

## Interface

Interfaces are the basic building block of libhal and enables the
flexibility needed to be portable and flexible.

An interface is a contract of functions that an implementing class must adhere
to. Interface documentation explains in detail the expected behavior that each
function should have on hardware regardless of the implementation. When a
program is compiled and a driver implements an interface, the compiler detects
if any of the functions have not been provided and if so, will report an error.

In libhal each interface corresponds to a type of embedded systems
primitive which can be things such as:

- Digital I/O (input/output pins)
- Analog to digital converter (adc)
- Pulse width modulation (pwm)
- Serial peripheral interface (spi)
- Universal asynchronous receiver transmitter (serial/uart)
- Accelerometer

## Peripheral drivers

Peripheral drivers are drivers for a target that is embedded within the device
and therefore cannot be removed from the chip and is fixed in number.

- Example: A digital output and input pin
- Example: 1 of 5 hardware timer within a micro-controller
- Example: Integrated analog-to-digital converter

## Device drivers

Are drivers for devices external to a target. In order to communicate with such
a device the target must have the necessary peripherals and peripheral drivers
to operate correctly.

- Example: an accelerometer driver for the mpu6050
- Example: a memory storage driver for a at581 flash memory
- Example: a black and white pixel display

## Soft drivers

Are drivers that do not have any specific underlying hardware associated with
them. They are used to emulate, give context to, or alter the behavior of
interfaces. For a driver to be a soft driver it must implement or have a way
to generate, construct or create implementations of hardware interfaces.

### Emulation Example

- Emulate spi by using 2 output pins and 1 input pin.
- Emulate uart transmission with a 16-bit spi driver and some clever bit
  positioning.

### Context Example

- Implement a rotary encoder by using an adc a potentiometer and some
  specification of the potentiometer like min and max angle, along with min and
  max voltage.
- Implement a dac using multiple output pins and a set of resistors and an op
  amp.

### Alteration example

- Implement an input pin that inverts the readings of an actual input pin
- Implement an i2c driver that is thread safe by taking an i2c and locking
  mechanism provided by the user.

In general, software drivers tend to incur some overhead so nesting them
deeply will effect performance.

## Hard drivers

Hard drivers are peripheral drivers and device drivers.

## Off Interface Function

Off interface functions are public class functions that a driver can have that
is beyond what is available for the interface it is implementing. These
functions usually configures a peripheral or device in a way that is outside of
the scope of the implementing interface. For peripherals these are platform
specific. For drivers these are device specific features. Examples of such
specific functions are as follows:

- An output pin driver with a high drain current mode
- An input pin driver with support for inverting the voltage level of what it
  reads in hardware.
- Enabling/disabling continuous sampling from an accelerometer where sampling
  continuously would make reading samples faster but would consume more power
  and disabling continuous sampling would do the opposite.

# ✍️ Usage

This section will go over how to use libhal in general. For details
pertaining to specific interfaces see the
[📚 Software APIs](https://libhal.github.io/libhal/) for more
details.

## Button & LED Example

Using an lpc4078:

```cpp
// Include driver code
#include <liblpc40xx/output_pin.hpp>
#include <liblpc40xx/input_pin.hpp>

int main() {
  // Get pin P0[2] as an output pin
  hal::output_pin & led = hal::lpc40xx::output_pin::get<0, 2>();
  // Get pin P1[6] as an input pin
  hal::input_pin & button = hal::lpc40xx::input_pin::get<1, 6>();

  while (true)
  {
    if (button.level().value() == true) {
      led.level(true);
    } else {
      led.level(false);
    }
  }
}
```

NOTE: that normally you wouldn't just get the value out of the function
`button.level()` using the `::value()` function because this exits the
application if the response contains an error. Luckily
`hal::lpc40xx::input_pin` never returns an error so the error handling check
can be ignored.

## Blink Example

Using stm32f10x:

```cpp
#include <chrono>

#include <libarmcortex/counter.hpp>
#include <libhal/counter/util.hpp>
#include <libstm32f10x/output_pin.hpp>
#include <libarmcortex/dwt_counter.hpp>

int main() {
  // Get pin A2 as an output pin
  hal::output_pin & led = hal::stm32f10x::output_pin::get<'A', 2>();
  // Construct a hardware counter
  hal::counter & counter = hal::cortex_m::dwt_counter::get(
      hal::stm32f10x::clock::cpu());

  while (true)
  {
    using std::chrono::literals;
    led.level(true);
    hal::delay(counter, 500ms);
    led.level(false);
    hal::delay(counter, 500ms);
  }
}
```

## Library file structure

libhal attempts to keep the organization of source code simple and
consistent in order to make including libraries easy to remember.

The file organization follows these rules:

1. Only 2 layers deep, excluding the `internal/` directory.
2. Non-hardware related utilities are placed at the root of the directory.
3. Each interface has a directory at the root of the libhal directory.
4. Each interface directory will have an `interface.hpp` file.
    - Example: `#include <libhal/adc/interface.hpp>`
    - Example: `#include <libhal/dac/interface.hpp>`
5. Any files associated/extending a particular interface will reside in that
   interfaces directory such as soft drivers or utilities.
6. Any hardware/interface files that extend to multiple interfaces will be
   placed in one interface directories. The choice should be the directory that
   makes the most sense, but this can be very arbitrary.
    - Example: `#include <libhal/input_pin/pin_resistor.hpp>`, this file
      could be in `output_pin` or `interrupt_pin` but `input_pin` seems like the
      best choice but is effectively arbitrary.

```
libhal/
├── config.hpp
├── <interface_1> (the name of an example interface)
│   ├── interface.hpp (REQUIRED: the interface definition is found here)
│   ├── utility_class.hpp (some interfaces have utility classes as well)
│   ├── mock.hpp (mocks for unit testing can be found here)
│   ├── unit.hpp (contains any units associated with the interface)
│   └── util.hpp (utilities for the interface can be found here)
├── i2c (example interface)
│   ├── interface.hpp (holds the hal::i2c interface)
│   ├── thread_safe.hpp (holds a soft driver implementing hal::i2c but with lock support)
│   └── util.hpp (holds hal::i2c utilities)
├── internal (internal code that should NOT be accessed directly)
│   └── third_party (dependencies for libhal)
│       ├── leaf.hpp (add Boost.LEAF for error handling and transport)
│       ├── uintwide_t.h (add support for integers above 64-bits in width)
│       └── units (add physical unit support)
├── enum.hpp (utility to handle enumerations)
├── error.hpp (error handling code is found here)
├── frequency.hpp (definition of the frequency type is found here)
├── math.hpp (helper math functions)
├── overflow_counter.hpp (detecting counter overflow)
├── percent.hpp (defines the percent class)
├── static_callable.hpp (convert polymorphic functions into free functions, useful for ISRs)
├── static_memory_resource.hpp (static buffer for pmr containers)
├── testing.hpp (utilities for unit testing)
└── to_array.hpp (contains conversions from containers to std::array)
```

## Using Utility Functions

Utility functions help eliminate boilerplate code for the application and driver
writers. They provide common semantics for drivers such as being able to
call `hal::read(/* insert interface here */)` on interfaces that have
read/sample capabilities.

Meaning that the following code should work for all three of these functions.

```cpp
constexpr hal::byte address(0x17);
auto response_i2c = hal::read<1>(i2c, address);
auto response_spi = hal::read<1>(spi);
auto response_uart = hal::read<1>(uart);
```

Utility functions are always "free" functions. "Free" means a non-class member
function. Utility functions should never need access to the internal details of
a class and thus do not and should not be members of the class.

When C++ adds support for UFCS (Uniform function call syntax) free functions can
be called as if they were member functions and class functions could be called
as if they were free functions. Slated currently for C++26.

An example of UFCS would be the following:

```cpp
// These two will be equivalent in C++26
auto c_style_call = hal::read<1>(spi);
auto ufcs_style_call = spi.read<1>();
```

### Finding Utilities

Utility headers can be found within interface folders with the name `util.hpp`.
For example if you want to use utilities for `hal::adc` then you would
include `#include <libhal/adc/util.hpp>`.

### Common Utility Functions

To keep the semantics consistent almost every driver will have either or both a
`hal::read()` or `hal::write()` free function. These functions should do
what a typical developer should expect, read from the device for input devices
or write to the device for output devices. The exact behavior depends on the
interface.

To find more, see the
[📚 Software APIs](#📚-software-apishttpslibhalgithubiolibhal).

## Using Device Drivers

Instantiating a device driver is different from a peripheral driver because
device drivers require other drivers in order to operate. Sensors tend to need
i2c or spi. GPS modules generally require serial. Motor controllers generally
need pwm signals and some output pins.

### Using an MPU6050 as an Accelerometer and/or Gyroscope

The following steps assumes you already have a project that can be compiled and
flashed on to a device and also has driver support for i2c.

This particular examples uses the "normal" or low bandwidth versions of the
mpu6050 driver and not the high bandwidth version that is more complicated.

#### Step 1. Install the libmpu6050

In order to get started we need install libmpu6050 via conan.

```bash
conan install libmpu6050
```

#### Step 2. Add as dependency to conan file

TBD

#### Step 3. Include & instantiate the mpu6050 driver in your project

Follow along with the example code below

```cpp
#include <libexamplemcu/i2c.hpp>
#include <libmpu6050/mpu6050.hpp>

int main() {
  hal::i2c & i2c = /* some i2c driver provided here */;
  // Create an mpu6050 driver and pass the i2c associated with the physical i2c
  // bus that is connected to the mpu6050's SDA and SCL lines.
  hal::mpu6050 mpu6050(i2c);

  // Get a reference to the accelerometer portion of the mpu6050
  hal::accelerometer & accelerometer = mpu6050.as_accelerometer();
  // Get a reference to the gyroscope portion of the mpu6050
  hal::gyroscope & gyroscope = mpu6050.as_gyroscope();

  // Read a sample from the acceleration of the mpu6050
  auto accelerometer_sample = accelerometer.read();
  // Read the rotational velocity of the mpu6050
  auto gyroscope_sample = gyroscope.read();

  /* Do other work... */
}
```

#### Step 4. DONE!

At this point you have a fully functional and available accelerometer and
gyroscope drivers that your code can use.

## Using Soft Drivers

Using soft drivers is no different than using a device driver. The only
difference between device drivers and soft drivers is the fact that soft drivers
are not associated with a particular device like an mpu6050 or esp8266. They are
generic.

A useful soft driver that can be used when a target does not have an spi
peripheral or cannot use one of the available spi busses, is the
`hal::bit_bang_spi`. "bit bang" refers to any method of data transmission that
employs software as a substitute for dedicated hardware to generate transmitted
signals or process received signals. `hal::bit_bang_spi` implements the
`hal::spi` interface using 2 `hal::output_pins` and 1 `hal::input_pin`.

Being software emulated this driver is far slower than using hardware driven
spi.

```cpp
#include <libhal/spi/bit_bang.hpp>
#include <liblpc40xx/output_pin.hpp>
#include <liblpc40xx/input_pin.hpp>

int main() {
  // Get references to all of the pins you want to use for spi emulation
  hal::output_pin & clock = hal::lpc40xx::output_pin::get<0, 1>();
  hal::output_pin & data_out = hal::lpc40xx::output_pin::get<0, 2>();
  hal::input_pin & data_in = hal::lpc40xx::input_pin::get<0, 3>();
  // Get an output_pin and have it act like a chip select
  hal::output_pin & chip_select = hal::lpc40xx::output_pin::get<0, 4>();

  // Construct the bit_bang_spi object using the implementations above
  hal::bit_bang_spi bit_bang_spi(clock, data_out, data_in);

  std::array<hal::byte, 4> payload = {
    hal::byte(0x11),
    hal::byte(0x22),
    hal::byte(0x33),
    hal::byte(0x44),
  };

  chip_select.level(false);
  hal::write(bit_bang_spi, payload);
  chip_select.level(true);

  return 0;
}
```

This can go even further. You don't need to use pins directly connected to the
micro-controller. You could even use pins from a device driver such as an I/O
expander:

```cpp
#include <libhal/spi/bit_bang.hpp>
#include <liblpc40xx/output_pin.hpp>
#include <liblpc40xx/input_pin.hpp>

int main() {
  hal::i2c & i2c0 = hal::lpc40xx::i2c::get<0>();
  hal::pca9536 io_expander(i2c0);

  // Get references to all of the pins you want to use for spi emulation
  hal::output_pin & clock = io_expander.get_as_output_pin<1>();
  hal::output_pin & data_out = io_expander.get_as_output_pin<2>();
  hal::input_pin & data_in = io_expander.get_as_input_pin<3>();
  hal::output_pin & chip_select = io_expander.get_as_output_pin<4>();

  // NOTICE: That the code below doesn't have to change even if the pin
  // implementations change.

  // Construct the bit_bang_spi object using the implementations above
  hal::bit_bang_spi bit_bang_spi(clock, data_out, data_in);

  // Get an output_pin and have it act like a chip select

  std::array<hal::byte, 4> payload = {
    hal::byte(0x11),
    hal::byte(0x22),
    hal::byte(0x33),
    hal::byte(0x44),
  };

  chip_select.level(false);
  hal::write(bit_bang_spi, payload);
  chip_select.level(true);

  return 0;
}
```

## 📊 Utility Classes

Utility classes are like soft drivers except they do not implement hardware
interfaces. Utility classes are generally used to manage an interface and
extend a driver's usefulness.

Examples of this would be `hal::can_network` which takes an `hal::can`
implementation and manages a map of the messages the device has received on the
can bus.

Another example is `hal::uptime_counter` which takes an `hal::counter` and
for each call for uptime on the uptime counter, the class checks if the 32-bit
counter has overflowed. If it has, then increment another 32-bit number with the
number of overflows counted. Return the result as a 64-bit number which is the
concatenation of both 32-bit numbers. This driver, so long as it is checked
often enough, can take a 32-bit hardware counter and extend it to a 64-bit
counter.

### hal::percent

(TODO)

### hal::frequency

(TODO)

## ⚖️ Using mp-units with libhal

(TODO)

## ☔️ Handling errors

Errors are handled in libhal using
[Boost.LEAF](https://boostorg.github.io/leaf/). Check out their documentation
for details on how to use it in detail. It is generally favorable to enable
embedded mode for LEAF as it greatly reduces the storage and memory requires of
the system.

```cpp
// Define this at the top of your main application file or in your compiler
// arguments.
#define BOOST_LEAF_EMBEDDED
// If you aren't using threads then add this as well
#define BOOST_LEAF_NO_THREADS
```

LEAF also allows you to control how exceptions are handled by defining a
`boost::throw_exception(std::exception const&)` function. In general you want
this to simply execute `std::exit` when this occurs. To do this, simply add
this snippet to one of the C++ files linked into the project.

```cpp
namespace boost {
void throw_exception(std::exception const& e)
{
  std::exit();
}
```

### Basic errors

```cpp
#define BOOST_LEAF_EMBEDDED
#define BOOST_LEAF_NO_THREADS

#include <array>
#include <span>

#include <libhal/i2c/util.hpp>
#include <liblpc40xx/i2c.hpp>

int main()
{
  // Get an i2c peripheral implementation
  auto& i2c0 = hal::lpc40xx::i2c::get<0>();

  // Default configure the i2c0 bus (100kHz clock)
  i2c0.configure({});

  hal::attempt_all(
    // First function can be considered the "try" portion of the code. If an
    // error result is returned from this function the handlers below will be
    // called.
    [&i2c0]() -> status {
      constexpr hal::byte address(0x11);
      std::array<hal::byte, 1> dummy_payload{ hal::byte{ 0xAA } };
      // Functions that return boost::leaf::result must have their result
      // checked and handled. To do this we use the HAL_CHECK to remove
      // the boiler plate in doing this.
      //
      // To make sure that errors are transported up the stack each call to a
      // function returning a boost::leaf::result must be wrapped in a
      // HAL_CHECK() macro call.
      HAL_CHECK(hal::write(i2c, address, dummy_payload));
      return {};
    },
    // Functions after the first are the handlers.
    // In this case, we only check for hal::i2c::errors.
    [](std::errc p_error) {
      switch(p_error) {
        case std::errc::no_such_device_or_address:
          // Handle this case here...
          break;
        case std::errc::io_error:
          // Handle this case here...
          break;
      }
    },
    // A function that takes no parameters is the wild card and is called when
    // there are unhandled remaining errors
    []() {
      // Unknown error occurred!
      // Handle those here!
    });

  return 0;
}

// This is here to remove exceptions from being thrown
namespace boost {
void throw_exception(std::exception const& e)
{
  std::exit();
}
} // namespace boost
```

### Getting logs from errors

(TODO)

### Peripheral driver debug snapshots

(TODO)

### Getting stack traces

(TODO)

## 🎛️ Customization

libhal uses `tweak.hpp` header files for customization and
configuration. See [A New Approach to Build-Time Library
Configuration](https://vector-of-bool.github.io/2020/10/04/lib-configuration.html)
for more details.

```cpp
#pragma once
#include <string_view>
namespace hal::config {
// Defaults to "test". Indicates that the current running platform is a
// unit/integration test. Change this to the target platform you are building
// for. For example, if you are targeting the LPC4078 chip, you should change
// this to "lpc4078".
constexpr std::string_view platform = "test";
// Defaults to "true". Enables stack tracing when errors do occur. There is a
// performance cost, albeit small, to capturing the current function name.
constexpr bool get_stacktrace_on_error = true;
// Defaults to "32". The maximum depth a stack trace can reach before it stops
// adding entries to the stack trace. Changing this effects the amount of space
// that the hal::stacktrace object takes up in a functions stack when used
// with Boost.LEAF.
constexpr size_t stacktrace_depth_limit = 32;
// Defaults to "false". If set to false, only the fully qualified function name
// will be stored in the stack trace. Set to true, the stack trace will capture
// the line number and file name into the stack trace object as well. Capturing
// the file names will increase the binary size of the application as the file
// name strings need to be stored in ROM.
constexpr bool get_source_position_on_error = false;
}  // namespace hal::config
```

Create a `libhal.tweak.hpp` file somewhere in your application and make
sure it is within one of the compiler's include paths. For GCC/Clang you'd use
the `-I` flag to specify directories where headers can be found. The file must
be at the root of the directory listed within the `-I` include path.

# Techniques

## Virtual Static Polymorphism (VSP)

TL;DR: This technique is used to eliminate the cost of making virtual function
calls.

### Discussion

Each interface in libhal uses the keyword `virtual` to support runtime
polymorphism. There are consequences to using the `virtual` keyword such as the
generation of a "vtable". This article
[Demystifying virtual functions, Vtable and VPTR in
C++](https://medium.com/@abhichavhan/demystifying-virtual-functions-vtable-and-vptr-in-c-bf56f11f7cc7)
explains how vtables work in detail.

Whether or not vtables use too much space for an application is up for debate
depending on the application. libhal mitigates this by trying to keep
the number of virtual functions for each interface as small as is reasonable.

The real concern regarding `virtual` keyword use is the function call
performance. In order to call a virtual function, a lookup must be performed,
then the call can be made. This tends to require 1 to 2 additional instructions
before a function is called. For most applications this is negligible but for
those in which this is a deal breaker there is a solution in using the "Virtual
Static Polymorphism" technique. Note that this technique can improve call speed
but at the cost of increasing the binary size of the application.

### Writing Device Drivers with this Technique

Here is an example of a soft driver for `hal::input_pin` which inverts the
value of the read function using VSP.

```cpp
namespace hal
{
template<typename T = hal::input_pin>
class invert_read : public hal::input_pin {
public:
  template<typename U>
  invert_read(U & p_input_pin) : m_input_pin(p_input_pin) {}

private:
  status driver_configure(
    const settings& p_settings) noexcept override
  {
    return m_input_pin->configure(p_settings);
  }

  result<bool> driver_level() noexcept override
  {
    return !HAL_CHECK(m_input_pin->level());
  }

  T * m_input_pin;
};
}
```

How is this useful? See the breakdown.

### Scenario #1: Virtual call

In this scenario, the default class template type has not been explicitly
changed and thus the code will call class functions in a virtual, indirect way.

```cpp
hal::some_mcu::input_pin & input0 = hal::some_mcu::get_input_pin<0>();
hal::invert_pin runtime_polymorphic(input0);
auto result0 = runtime_polymorphic.read();
```

The information about the original class object and its internal implementation
is not visible to the `runtime_polymorphic` object. So when read is called,
because the type of the internal pointer is `T = hal::input_pin`, the code
must perform a virtual call through the interface.

### Scenario #2: Direct call

Now lets look at a scenario where the default class template type has been
explicitly set to the type of the input pin driver.

```cpp
// Uses static (direct) function calls
hal::some_mcu::input_pin & input1 = hal::some_mcu::get_input_pin<1>();
hal::invert_pin<hal::some_mcu::input_pin> static_polymorphic(input1);
auto result1 = runtime_polymorphic.read();
```

Now `hal::invert_pin` is no longer dealing with an interface as type `T` is
now `hal::some_mcu::input_pin`. As far as `hal::invert_pin` is concerned, we
never used an interface in this case. Note that the constructor's type `U` is
now equal to the type `T` and thus there is no down casting occurring. Now when
read is called, the class has full context regarding the implementation of the
read function. The compiler can then make a decision on whether or not to do the
following three options:

1. **Worst Case Scenario**: virtual call (costly so unlikely)
2. **Better Scenario**: if the function's implementation is sufficiently large,
   direct function call. This does result in cost but its better than a virtual
   call.
3. **Best Case Scenario**: if the function's implementation is small enough, the
   compiler can inline the implementation of the driver into the soft driver
   removing the call entirely.

### Evaluating the PROS & CONS

The PROS of scenario 2 or 3 from the list above is that you get better calling
performance. And if you stack these multiple levels deep the performance
improves stack.

The CONS of this is that for each different unique explicit instantiation of
`hal::invert_pin`, there will be multiple implementations of the same driver
in the binary. For example, if a project has 3 drivers that implement the input
pin interface and each requires an `hal::invert_pin` class to invert their
read values, then you would have the following:

```cpp
// using virtual calls
hal::invert_pin<hal::input_pin>
// direct calls to some_mcu::input_pin
hal::invert_pin<hal::some_mcu::input_pin>
// direct calls to io_expander::input_pin
hal::invert_pin<hal::io_expander::input_pin>
```

The cost of all of these driver instantiations can be large for large projects
if the choice of speed over space is not made carefully.

## Supporting Multiple Platforms

Peripherals are platform specific, thus an lpc40xx output pin driver will not
work on an stm32f10x device. Because each implements the peripherals in a
different and unique way there needs to be a separate driver for each. This is
the crux of why embedded software is not portable across multiple devices. But
libhal has a method of fixing this.

The following section of code will explain in its comments how to support
multiple platforms between lpc40xx and stm32f103

```cpp
int main()
{
  // Step 1. Create a set of interface pointers to each driver your application
  //         will needed.
  hal::input_pin * button{};
  hal::output_pin * led{};

  // Step 2. Map each pointer to their respective peripheral on either device.
  //         `if constexpr` is required to prevent leaking implementation
  //         details from one platform to another. This is important because a
  //         lpc40xx driver can never work on stm32f10 and thus leaking code
  //         into a binary meant for another platform results in code bloat.
  if constexpr (hal::is_platform("lpc40"))
  {
    button = &hal::lpc40xx::input_pin<0, 1>();
    led = &hal::lpc40xx::output_pin<0, 2>();
  }
  else if (hal::is_platform("stm32f10"))
  {
    button = &hal::stm32f103::input_pin<'A', 1>();
    led = &hal::stm32f103::output_pin<'B', 2>();
  }
  else
  {
    return -1;
  }

  // Step 3. Use the interface pointers above.
  while(true)
  {
    if (button.read().value())
    {
      (void)led.level(true);
    }
  }
}
```

# 🪪 Library Badges

![supported](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Flibhal%2Flibhal%2Fmain%2Fbadges%2Fsupported.json)
![safety critical](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Flibhal%2Flibhal%2Fmain%2Fbadges%2Fsafety_critical.json)
![allocates](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Flibhal%2Flibhal%2Fmain%2Fbadges%2Fallocates.json)
![floats](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Flibhal%2Flibhal%2Fmain%2Fbadges%2Ffloats.json)
![throws](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Flibhal%2Flibhal%2Fmain%2Fbadges%2Fthrows.json)

The badges above are displayed in a library's README.md right below the title to
indicate attributes of the library. When searching for a library to use for your
project, these badges can help you decide if the project meets your
requirements.

## Supported

All libraries that implement libhal interfaces should have this badge to
indicate library users/consumers.

## 🦺 SAFETY CRITICAL

If a library follows completely the AUTOSAR C++20 guidelines.

## ⚠️ ALLOCATES

This badge is placed for a libraries that have the possibility to dynamically
allocates memory via `new`, `malloc`, `std::allocator` or a standard library
that uses any of the allocating functions.

## ⚠️ FLOATS

If a library uses floating point arithmetic anywhere in its implementation.
Some points to consider when seeing a library that uses floats:

- Floating point operations take up flash space for devices without a hardware
  FPU (floating point unit).
- The cost is only paid once because the software floating point support code
  can be called each time for each operation. So if you are already using
  software floats then it is likely that using this library will not add more
  space to a project.
- Software driven floating arithmetic is very slow compared to integer
  arithmetic.
- For applications using multiple threads along with FPU support, context
  switching time will increase because the FPU registers will need to be saved.

## ⚠️ THROWS

If a library ever throws an exception anywhere in its code base.
Some points to consider when seeing a library that uses floats:

- Exceptions can be quite slow to propagate when they occur, although this is
  not always the case.
- If a project has exceptions turned off, this library will not compile, and thus
  cannot be used.
- Using exceptions generally incurs an increase in size for your binary (around
  8kB for the exception runtime environment for arm cortex-m).
- The infrastructure for error handling is already handled by Boost.LEAF so
  exceptions are redundant.
- Exceptions require dynamic memory allocate.

# 📜 Library Standards

All guides follow the [C++ Core
Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines).

## Making your own libhal Interfaces and Drivers

In order to demonstrate how to create an interface a thoroughly
documented/commented set of example code has been written. The comments in these
files acts as guides to help new developers learn how to create their own
libhal interfaces, peripheral drivers, device drivers and soft drivers.

The following files are a guide to how to write their respective driver:

- Peripheral Drivers:
  - TBD: [`lpc40xx:adc.hpp`]()
- Device Drivers:
  - TBD: [`mma8452q:accelerometer.hpp`]()
- Soft Drivers;
  - TBD: [`libhal:rc_servo.hpp`]()

Each of these files will include comments that document every portion of the
code and explain why each line or block of code is present in the code.

## 🎨 Style

- Code shall follow libhal's `.clang-format` file, which uses the
  Mozilla C++ style format as a base with some adjustments.
- Code shall follow libhal's `.naming.style` file, which is very
  similar to the standard library naming convention:
  - CamelCase for template parameters.
  - CAP_CASE for macros.
  - lowercase snake_case for everything else.
  - prefix `p_` for function parameters.
  - prefix `m_` for private/protected class member.
- Refrain from variable names with abbreviations where it can be helped. `adc`
  `pwm` and `i2c` are extremely common so it is fine to leave them as
  abbreviations. Most people know the abbreviations more than the words that
  make them up. But words like `cnt` should be `count` and `cdl` and `cdh`
  should be written out as `clock_divider_low` and `clock_divider_high`.
  Registers do get a pass if they directly reflect the names in the data sheet
  which will make looking them up easier in the future.
- Use `#pragma once` as the include guard for headers.
- Every file must end with a newline character.
- Every line in a file must stay within a 80 character limit.
  - Exceptions to this rule are allowed. Use `// NOLINT` in these cases.
- Radix for bit manipulation:
  - Only use binary (`0b1000'0011`) or hex (`0x0FF0`) for bit manipulation.
  - Never use decimal or octal as this is harder to reason about for most
    programmers.
- Every public API must be documented with the doxygen style comments (CI will
  ensure that every public API is documented fully).
- Include the C++ header version of C headers such as `<cstdint>` vs
  `<stdint.h>`.

## 🚫 Coding Restrictions

### Refrain from performing manual bit manipulation

Use the `libxbitset` library to perform bitwise operations operations.

### Refrain from using MACROS

Only use macros if something cannot be done without using them. Usually macros
can be replaced with constexpr or const variables or function calls. A case
where macros are the only way is for HAL_CHECK() since there is no way
to automatically generate the boiler plate for returning if a function returns
and error in C++ and thus a macro is needed here to prevent possible mistakes
in writing out the boilerplate.

Only use preprocessor `#if` and the like if it is impossible to use
`if constexpr` to achieve the same behavior.

### Never include <iostream>

Never include `<iostream>`.

#### Rationale

Applications incurs an automatic 150kB space penalty for including this header
even if the application never uses any part of `<iostream>`.

### Refrain from memory allocations

Drivers should refrain from memory allocate as much as possible that includes
using STL libraries that allocate such as `std::string` or `std::vector`.

#### Rationale

TBD

### Drivers should not log to STDOUT or STDIN

Peripheral driver must NOT log to stdout or stderr. This means no calls to

- `std::printf`
- `std::cout`
- `std::print` (C++26 using std::format)

#### Rationale

Consider using the file I/O libraries in C, C++, python or some other
language. Would you, as a developer, ever imagine that opening, reading,
writing, or closing a file would to your console? Especially if there did not
exist a way to turn off logging. Most users would be very upset as this would
not seem like the role of the file I/O library to spam my console. This gets
even worse if a particular application has thousands of files and each operation
is logging.

The role of logging should be held by the application developer, not their
drivers or helper functions, unless the purpose of the helper functions or
driver is to write to console.

Instrumenting a driver with log statements IS allowable provided that they are
controllable via a tweak configuration and that they are turned off by default
AND turning off this configuration eliminates all logging code from the driver.

Example of logging where logging can be disabled:

```cpp
void my_driver_log([[maybe_unused]] std::string_view p_str,
                   [[maybe_unused]] uint32_t argument)
{
  if constexpr (hal::lpc40xx::config::enable_logging)
  {
    std::printf("%s : %" PRIu32, p_str.data(), argument);
  }
}

// ...

my_driver_log("Control Register 1 = ", reg->control1);
```

Notice how, if `enable_logging` is false, the function body is empty and thus,
the compiler will find it easy to eliminate the function itself. The parameters,
specifically the `std::string_view` will be garbage collected at compile time
since its use is a no-op.

### Drivers should not purposefully halt the application

Drivers are not entitled to halt the execution of the application and thus any
code block that would effectively end or halt the execution of the program
without giving control back to the application are prohibited.

As an example drivers should never call:
  - `std::abort()`
  - `std::exit()`
  - `std::terminate()`
  - any of their variants

This includes placing an **infinite loop block** in a driver.

#### Rationale

An application should have control over how their application ends. A
driver should report severe errors to the application and let the application
decide the next steps. If a particular operation cannot be executed as intended,
then `hal::new_error()` should be called.

Constructors would be the only valid place to put an exit statement, because
they cannot return errors only themselves.

The solution to this is to use a factory function like so:

```cpp
class device_driver {
  public:
    result<device_driver> create(/* ... */) {
      // Perform operations that may fail here
      return device_driver(/* ... */);
    }

  private:
    device_driver(/* ... */) {
      // Constructors should never fail and thus any work done here must not
      // fail.
    }
};
```

For peripherals:

```cpp
class peripheral_driver {
  public:
    // Since peripherals are constrained and have a finite set of values
    // This also ensures that the driver is only constructed once and afterwards
    // simply returns back a reference to that object.
    template<size_t PortNumber>
    // NOTE: Returns a reference not an object.
    //       Objects are owned by the create function
    result<peripheral_driver&> create(/* ... */) {
      // Perform operations that may fail here
      static peripheral_driver driver(/* ... */);
      return driver;
    }

  private:
    peripheral_driver(/* ... */) {
      // ...
    }
};
```

### Drivers should not pollute the global namespace

All drivers must be within the `hal` namespace (RECOMMENDED) or within their
own bespoke namespace.

Inclusion of a C header file full of register map structures is not allowed as
it pollutes the global namespace and tends to result in name collisions.

Care should be taken to ensure that the `hal` namespace is also as clean as
possible by placing structures, enums, const data, and any other symbols into
the driver's class's namespace like so:

```cpp
namespace hal::target
{
class target {
  struct register_map {
    std::uint32_t control1;
    std::uint32_t control2;
    std::uint32_t data;
    std::uint32_t status;
    // ..
  };

  struct control1_register {
    static constexpr auto channel_enable = xstd::bitrange::from<0, 7>();
    static constexpr auto peripheral_enable = xstd::bitrange::from<8>();
    // ...
  };

  // ...
};
}
```

### Interface should follow the public private API

See [private virtual method](http://www.gotw.ca/publications/mill18.htm)
for more details. Rationale can be found within that link as well.

# 📚 Libraries

## Processors

- [libarmcortex](https://github.com/libhal/libarmcortex): Drivers for
  the ARM Cortex M series of processors.
- [libriscvi32](https://github.com/libhal/libriscvi32): Coming soon.
  Drivers for 32-bit RISC-V processors.

## Targets

- [liblpc40xx](https://github.com/libhal/liblpc40xx): Drivers the lpc40xx
  series of microcontrollers. This includes startup code, linker scripts, and
  peripheral drivers.
- [libstm32f1xx](https://github.com/libhal/libstm32f1xx): Coming soon.
  Drivers the stm32f1xx series of microcontrollers.

### Drivers

- [libesp8266](https://github.com/libhal/libesp8266): Drivers for the
  esp8266 micro-controller as well as drivers for the WiFi/Internet firmware AT
  client.
- [libmpu6050](https://github.com/libhal/libmpu6050): Coming soon.
  Accelerometer and gyroscope device. Requires an i2c driver.
- [libatmega328](https://github.com/libhal/libatmega328): Coming soon.
  Drivers for atmega328 micro-controller.


# 💡 Motivation

The world of embedded systems is written almost entirely in C and C++. More and
more the embedded world moves away from C and towards C++. This has to do with
the many benefits of C++ such as type safety, compile time features,
meta-programming, multiple programming paradigms which, if use correctly can
result in smaller and higher performance code than in C.

But a problem for embedded software in C++, as well as in C, is that there isn't
a consistent and common API for embedded libraries. Looking around, you will
find that each hardware vendor has their own set of libraries and tools for
their specific products. If you write a driver on top of their libraries, you
will find that your code will only work for that specific platform/product. In
some cases you may also be limited to just their toolchain. You as the developer
are locked in to this one specific setup. And if you move to another platform,
you must do the work of rewriting all of your code again.

libhal seeks to solve this issue by creating a set of generic interfaces
for embedded system concepts such as serial communication (UART), analog to
digital conversion (ADC), inertial measurement units (IMU), pulse width
modulation (PWM) and much more. The advantage of building a system on top of
libhal is that higher level drivers can be used with any target platform
whether it is an stm32, an nxp micro controller, an RISC-V or is on an
embedded linux.

This project is inspired by the work of Rust's embedded_hal and follows many of
the same design goals.

libhal's design goals:

1. Serve as a foundation for building an ecosystem of platform agnostic drivers.
2. Must abstract away device specific details like registers and bitmaps.
3. Must be generic across devices such that any platform can be supported.
4. Must be minimal for boosting performance and reducing size costs.
5. Must be composable such that higher level drivers can build on top of these.
6. Be accessible through package mangers so that developers can easily pick and
   choose which drivers they want to use.

# 👥 Contributing

## 🤚 Self Assigning to an Issue

If you find an issue you'd like to work on, simply type and submit a comment
with the phrase `.take` in it to get assigned by our github actions.

## 📤 Pull Request Guidelines

1. Code must finish continuous integration steps before it will be reviewed.
2. Commit messages should follow these guidelines here
   https://cbea.ms/git-commit/.
3. Pull requests should contain a single commit
4. Pull requests should be small and implement a single feature where possible.
   If it can be broken up into separate parts it most likely should be.
5. Each PR should have an associated issue with it. Exceptions are made for very
   small PRs such as fixing typos, fixing up documentation or removing
   unnecessary headers.
