<p align="center">
  <img height="150" src="docs/chip.svg">
</p>

# libembeddedhal

[![Discord](https://img.shields.io/discord/800515757871726622?color=7389D8&logo=discord&logoColor=ffffff&labelColor=6A7EC2)](https://discord.gg/p5A6vzv8tm)
[![docs](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/docs.yml/badge.svg?branch=main)](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/docs.yml)
[![lint](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/lint.yml/badge.svg?branch=main)](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/lint.yml)
[![tests](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/tests.yml/badge.svg?branch=main)](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/tests.yml)

**🏗️ WARNING: PROJECT IN DEVELOPMENT 🚧**

| [Overview](#overview)
| [Install](#install)
| [Motivation](#motivation)
| [Policies](#policies)
| [User Guides](#user-guides)
| [Libraries](#libraries)
| [Developer Guides](#developer-guides)
| [FAQ](#faq)
|

# [📚 Software APIs](https://libembeddedhal.github.io/libembeddedhal/)

# 📥 Install

Install from conan:

```bash
conan install libembeddedhal
```

Installing from source locally:

```bash
git clone https://github.com/SJSU-Dev2/libembeddedhal.git
cd libembeddedhal
conan create .
```

# ℹ️ Overview

libembeddedhal exists to make hardware drivers:

- **🚚 portable**
- **🦾 flexible**
- **📦 accessible**
- **🍰 easy to use**

## Project Attributes

- Header only
- Available on Conan (coming soon to vcpkg)
- Does not throw exceptions
- Does not dynamically allocation
- Uses tweak header files for customization
- Designed to be modular, dynamic, composable, and lightweight
- Dependencies:
  - Boost.LEAF for error handling
  - C++20
  - libxbitset
  - mp-units
  - uintwide_t.h
- Target/platform agnostic (ARM Cortex, STM32, AVR, Embedded Linux, etc)
- Follows C++ Core Guidelines

# 📃 Glossary

Here is a list of terms used in libembeddedhal. It is HIGHLY RECOMMENDED that
those new to libembeddedhal to read this section.

## Target

Is a general purpose device that can execute code and contains 1 or more
peripherals or busses for communicating with devices and hardware external to
itself. In the context of libembeddedhal these are defined as a micro-controller
(MCU) or a system-on-chip (SOC).

## Interface

Is a codescription of the behavior that a hardware  of a C++ class without
committing to a particular implementation of that class.

## Peripheral drivers

Are drivers for a target that is embedded within the device and therefor cannot
be removed from the chip and is fixed in number.

- Example: A digital output and input pin
- Example: A hardware timer
- Example: An analog-to-digital converter

## Device drivers

Are drivers for devices external to a target. In order to communicate with such
a device the target must have the necessary peripherals and peripheral drivers
to operate correctly.

- Example: an accelerometer driver for the mpu6050
- Example: a memory storage driver for a at581 flash memory

## Hard drivers

Are simply peripheral drivers and device drivers.

## Soft drivers

Are drivers that do not have any specific underlying hardware associated with
them. They are used to emulate, give context to, or alter the behavior of other
interfaces. For a driver to be a soft driver it must implement or have a way
to generate, construct or create an implementation of a hardware interface.

- Emulation example: Using 2 output pins and 1 input pin to emulate spi
- Context example: Using an adc and the attributes of an attached
  potentiometer can be used to create a rotary encoder.
- Alteration example: Taking an input pin and inverting the logic level read
  from hardware.

In general, software drivers tend to incur some overhead so nesting them
deeply will effect performance.

## Off Interface Function

Is a public function that a driver can have that is beyond what is available for
the interface it is implementing. These functions usually configures a
peripheral or device in a way that is outside of the scope of the implementing
interface.

- Example: An output pin driver with a high drain current mode
- Example: An input pin driver with support for inverting the voltage level
  of what it reads in hardware.
- Example: Enabling/disabling continuous sampling from an accelerometer
  where sampling continuously would make reading samples faster but would
  consume more power and disabling continuous sampling would do the
  opposite.

# ✍️ Usage

This section will go over how to use libembeddedhal in general. For details
pertaining to specific interfaces see the
[📚 Software APIs](#📚-software-apishttpslibembeddedhalgithubiolibembeddedhal)
for more details.

## Button & LED Example

```C++
#include <liblpc40xx/output_pin.hpp>
#include <liblpc40xx/input_pin.hpp>

int main() {
  // Get pin A2 as an output pin
  embed::output_pin & led = embed::lpc40xx::output_pin::get<0, 2>();
  // Get pin B6 as an input pin
  embed::input_pin & button = embed::lpc40xx::input_pin::get<1, 6>();

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

## Blink Example

```C++
#include <chrono>

#include <libarmcortex/counter.hpp>
#include <libembeddedhal/counter/util.hpp>
#include <liblpc40xx/output_pin.hpp>

int main() {
  // Get pin A2 as an output pin (will be initialized after the first call)
  embed::output_pin & led = embed::lpc40xx::output_pin::get<0, 2>();
  // Get a hardware counter (will be initialized after the first call)
  embed::counter & counter = embed::lpc40xx::counter::get();

  while (true)
  {
    using std::chrono::literals;
    led.level(true);
    embed::wait_for(counter, 500ms);
    led.level(false);
    embed::wait_for(counter, 500ms);
  }
}
```

## Library file structure

libembeddedhal attempts to keep the organization of source code simple,
consistent in order to make including libraries easy to remember.

Some attributes of the file structure:

1. Only 2 layers deep, excluding the `internal/` directory.
2. Non-hardware related utilities are placed at the root of the directory.
3. Each interface has a directory at the root of the libembeddedhal directory.
4. Each interface directory will have an `interface.hpp` file.
    - Example: `#include <libembeddedhal/adc/interface.hpp>`
    - Example: `#include <libembeddedhal/dac/interface.hpp>`
5. Any files associated/extending a particular interface will reside in that
   interfaces directory.
6. Any hardware/interface files that extend to multiple interfaces will be
   placed in one interface directories. The choice should be which directory
   makes the most sense but this can be very arbitrary.
    - Example: `#include <libembeddedhal/input_pin/pin_resistor.hpp>`, this file
      could be in `output_pin` or `interrupt_pin` but `input_pin` seems like the
      best choice but is effectively arbitrary.

```
libembeddedhal/
├── config.hpp
├── <interface_1> (the name of an example interface)
│   ├── interface.hpp (REQUIRED: the interface definition is found here)
│   ├── utility_class.hpp (some interfaces have utility classes as well)
│   ├── mock.hpp (mocks for unit testing can be found here)
│   ├── unit.hpp (contains any units associated with the interface)
│   └── util.hpp (utilities for the interface can be found here)
├── i2c (example interface)
│   ├── interface.hpp (holds the embed::i2c interface)
│   ├── thread_safe.hpp (holds a soft driver implementing embed::i2c but with lock support)
│   └── util.hpp (holds embed::i2c utilities)
├── internal (internal code that should NOT be accessed directly)
│   └── third_party (dependencies for libembeddedhal)
│       ├── leaf.hpp (add Boost.LEAF for error handling and transport)
│       ├── uintwide_t.h (add support for numbers above 64-bit)
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
├── time.hpp (provides global time keeping functions)
└── to_array.hpp (converts containers to std::array)
```

## Using Utility Functions

Utility functions help eliminate boilerplate code for the application and driver
writers. They also provide common semantics for drivers such as being able to
call `embed::read(/* insert interface here */)` on any interface that has the
capability to read or sample something.

Utility functions are always "free" functions. "Free" meaning that the function
is standalone and is not a member of an object/class. Utility functions should
never need access to the internal details of a class and thus do not and should
not be members of the class.

When C++ adds support for UFCS (Uniform function call syntax) free functions can
be called as if they were member functions and class functions could be called
as if they were free functions. Slated currently for C++26.

An example of UFCS would be the following:

```C++
// What this function actually does is not very relevant, but what is relevant
// is that is a free function that can act in place of a class method.
template<size_t N, std::chrono::nanoseconds Spacing>
auto read(embed::adc & p_adc)
{
  // returns an array of N samples with "Spacing" amount of time between samples
}

void example_function(embed::adc & p_adc)
{
  using std::chrono::literals;
  // embed::adc does not have a templated read function in its interface, but
  // with UFCS the free "read" function will be looked up instead and if it
  // matches the parameters given here, it will be used.
  // In this case the first parameter is "p_adc".
  auto samples = p_adc.read<16, 100ms>();
  // do something with samples ...
}
```

### Finding Utilities

Utility headers can be found within interface folders with the name `util.hpp`.
For example if you want to include the utilities for `embed::adc` then you would
include `#include <libembeddedhal/adc/util.hpp>`.

### Common Utility Functions

To keep the semantics consistent almost every driver will have either or both a
`embed::read()` or `embed::write()` free function. These functions should do
what a typical developer should expect, read from the device for input devices
or write to the device for output devices. The exact behavior depends on the
interface.

The read and write free functions are also overloaded to keep them consistent.
For example `embed::spi` has something like this:

```C++
[[nodiscard]] boost::leaf::result<std::byte> read(embed::spi & p_spi) noexcept
{
  // Read a single byte and return it.
}
```

and

```C++
template<size_t BytesToRead>
[[nodiscard]] boost::leaf::result<std::array<std::byte, BytesToRead>> read(
  spi& p_spi) noexcept
{
  // Read bytes into a buffer and return that buffer.
}
```

Both are overloads of each other and makes the semantics consistent for all read
like operations.

To find more, see the
[📚 Software APIs](#📚-software-apishttpslibembeddedhalgithubiolibembeddedhal).

## Using Device Drivers

Using a device driver requires that the target device you are working for
supports the peripherals for it. If necessary soft drivers can be used to
emulate peripherals.

### Using an MPU6050 as an accelerometer and/or gyroscope

The following steps assumes you already have a project that can be compiled and
flashed on to a device and also has driver support for i2c.

This particular examples uses the "normal" or low bandwidth versions of the
mpu6050 driver and not the high bandwidth version that is more complicated.

#### Step 1. Install the libmpu6050

In order to get started we need install libmpu6050 via conan.

```bash
conan install libmpu6050
```

#### Step 2. Include & Instantiate the mpu6050 driver in your project

Follow along with the example code below

```C++
#include <libexamplemcu/i2c.hpp>
#include <libmpu6050/mpu6050.hpp>

int main() {
  embed::i2c & i2c = /* some i2c driver provided here */;
  // Create an mpu6050 driver and pass the i2c associated with the physical i2c
  // bus that is connected to the mpu6050's SDA and SCL lines.
  embed::mpu6050 mpu6050(i2c);

  // Get a reference to the accelerometer portion of the mpu6050
  embed::accelerometer & accelerometer = mpu6050.as_accelerometer();
  // Get a reference to the gyroscope portion of the mpu6050
  embed::gyroscope & gyroscope = mpu6050.as_gyroscope();

  // Read a sample from the acceleration of the mpu6050
  auto accelerometer_sample = accelerometer.read();
  // Read the rotational velocity of the mpu6050
  auto gyroscope_sample = gyroscope.read();

  /* Do other work... */
}
```

#### Step 3. DONE!

At this point you have a fully functional and available accelerometer and
gyroscope drivers that your code can use.

## Using Soft Drivers

Using soft drivers is no different than using a device driver. The only
difference between device drivers and soft drivers is the fact that soft drivers
are not associated with a particular device like an mpu6050 or esp8266. They are
generic.

A useful soft driver that one can use in a pinch is the `embed::bit_bang_spi`.
"bit bang" refers to any method of data transmission that employs software as a
substitute for dedicated hardware to generate transmitted signals or process
received signals. `embed::bit_bang_spi` implements the `embed::spi` interface
using 2 `embed::output_pins` and 1 `embed::input_pin`.

Note that many soft drivers like `embed::bit_bang_spi`, due to its use of
bit-banging has lower performance compared to dedicated spi hardware. This
driver is helpful when you need spi but the device you are using does not
contain an spi peripheral. Another use case is that the spi busses available are
too saturated and a low speed low performance separate bus is needed.

```C++
#include <libembeddedhal/spi/bit_bang.hpp>
#include <liblpc40xx/output_pin.hpp>
#include <liblpc40xx/input_pin.hpp>

int main() {
  // Get references to all of the pins you want to use for spi emulation
  embed::output_pin & clock = embed::lpc40xx::output_pin::get<0, 1>();
  embed::output_pin & data_out = embed::lpc40xx::output_pin::get<0, 2>();
  embed::input_pin & data_in = embed::lpc40xx::input_pin::get<0, 3>();
  // Get an output_pin and have it act like a chip select
  embed::output_pin & chip_select = embed::lpc40xx::output_pin::get<0, 4>();

  // Construct the bit_bang_spi object using the implementations above
  embed::bit_bang_spi bit_bang_spi(clock, data_out, data_in);

  std::array<std::byte, 4> payload = {
    std::byte(0x11),
    std::byte(0x22),
    std::byte(0x33),
    std::byte(0x44),
  };

  chip_select.level(false);
  embed::write(bit_bang_spi, payload);
  chip_select.level(true);

  return 0;
}
```

This can go even further. You don't need to use pins directly connected to the
micro-controller. You could even use pins from an I/O expander:


```C++
#include <libembeddedhal/spi/bit_bang.hpp>
#include <liblpc40xx/output_pin.hpp>
#include <liblpc40xx/input_pin.hpp>

int main() {
  embed::i2c & i2c0 = embed::lpc40xx::i2c::get<0>();
  embed::pca9536 io_expander(i2c0);

  // Get references to all of the pins you want to use for spi emulation
  embed::output_pin & clock = io_expander.get_as_output_pin<1>();
  embed::output_pin & data_out = io_expander.get_as_output_pin<2>();
  embed::input_pin & data_in = io_expander.get_as_input_pin<3>();
  embed::output_pin & chip_select = io_expander.get_as_output_pin<4>();

  // NOTICE: That the code below doesn't have to change even if the pin
  // implementations change.

  // Construct the bit_bang_spi object using the implementations above
  embed::bit_bang_spi bit_bang_spi(clock, data_out, data_in);

  // Get an output_pin and have it act like a chip select

  std::array<std::byte, 4> payload = {
    std::byte(0x11),
    std::byte(0x22),
    std::byte(0x33),
    std::byte(0x44),
  };

  chip_select.level(false);
  embed::write(bit_bang_spi, payload);
  chip_select.level(true);

  return 0;
}
```

## 📊 Utility Classes

Utility classes are like soft drivers except they do not implement hardware
interfaces. Utility classes are generally used to manage an interface and
extend a driver's usefulness.

Examples of this would be `embed::can_network` which takes an `embed::can`
implementation and manages a map of the messages the device has received on the
can bus.

Another example is `embed::uptime_counter` which takes an `embed::counter` and
for each call for uptime on the uptime counter, the class checks if the 32-bit
counter has overflowed. If it has, then increment another 32-bit number with the
number of overflows counted. Return the result as a 64-bit number which is the
concatenation of both 32-bit numbers. This driver, so long as it is checked
often enough, can take a 32-bit hardware counter and extend it to a 64-bit
counter.

### embed::percent

(TODO)

### embed::frequency

(TODO)

## ⚖️ Using mp-units with libembeddedhal

(TODO)

## ☔️ Handling errors

Errors are handled in libembeddedhal using
[Boost.LEAF](https://boostorg.github.io/leaf/). Check out their documentation
for details on how to use it in detail. It is generally favorable to enable
embedded mode for LEAF as it greatly reduces the storage and memory requires of
the system.

```C++
// Define this at the top of your main application file or in your compiler
// arguments.
#define BOOST_LEAF_EMBEDDED
// If you aren't using threads then add this as well
#define BOOST_LEAF_NO_THREADS
```

LEAF also allows you to control how exceptions are handled by defining a
`boost::throw_exception(std::exception const&)` function. In general you want
this to simply execute `std::abort` when this occurs. To do this, simply add
this snippet to one of the C++ files linked into the project.

```C++
namespace boost {
void throw_exception(std::exception const& e)
{
  std::abort();
}
```

### Basic errors

```C++
#define BOOST_LEAF_EMBEDDED
#define BOOST_LEAF_NO_THREADS

#include <array>
#include <span>

#include <libembeddedhal/i2c/util.hpp>
#include <liblpc40xx/i2c.hpp>

int main()
{
  // Get an i2c peripheral implementation
  auto& i2c0 = embed::lpc40xx::i2c::get<0>();

  // Default configure the i2c0 bus (100kHz clock)
  i2c0.configure({});

  boost::leaf::try_handle_all(
    // First function can be considered the "try" portion of the code. If an
    // error result is returned from this function the handlers below will be
    // called.
    [&i2c0]() -> boost::leaf::result<void> {
      constexpr std::byte address(0x11);
      std::array<std::byte, 1> dummy_payload{ std::byte{ 0xAA } };
      // Functions that return boost::leaf::result must have their result
      // checked and handled. To do this we use the BOOST_LEAF_CHECK to remove
      // the boiler plate in doing this.
      //
      // To make sure that errors are transported up the stack each call to a
      // function returning a boost::leaf::result must be wrapped in a
      // BOOST_LEAF_CHECK() macro call.
      BOOST_LEAF_CHECK(embed::write(i2c, address, dummy_payload));
      return {};
    },
    // Functions after the first are the handlers.
    // In this case, we only check for embed::i2c::errors.
    [](embed::i2c::errors p_error) {
      switch(p_error) {
        case embed::i2c::errors::address_not_acknowledged:
          // Handle this case here...
          break;
        case embed::i2c::errors::bus_error:
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
  std::abort();
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

libembeddedhal uses the `tweak.hpp` header file approach to customization and
configuration. See [A New Approach to Build-Time Library
Configuration](https://vector-of-bool.github.io/2020/10/04/lib-configuration.html).

```C++
#pragma once
#include <string_view>
namespace embed::config {
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
// that the embed::stacktrace object takes up in a functions stack when used
// with Boost.LEAF.
constexpr size_t stacktrace_depth_limit = 32;
// Defaults to "false". If set to false, only the fully qualified function name
// will be stored in the stack trace. Set to true, the stack trace will capture
// the line number and file name into the stack trace object as well. Capturing
// the file names will increase the binary size of the application as the file
// name strings need to be stored in ROM.
constexpr bool get_source_position_on_error = false;
}  // namespace embed::config
```

Create a `libembeddedhal.tweak.hpp` file somewhere in your application and make
sure it is within one of the compiler's include paths. For GCC/Clang you'd use
the `-I` flag to specify directories where headers can be found. The file must
be at the root of the directory listed within the `-I` include path.

## 💡 Motivation

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

libembeddedhal seeks to solve this issue by creating a set of generic interfaces
for embedded system concepts such as serial communication (UART), analog to
digital conversion (ADC), inertial measurement units (IMU), pulse width
modulation (PWM) and much more. The advantage of building a system on top of
libembeddedhal is that higher level drivers can be used with any target platform
whether it is an stm32, an nxp micro controller, an RISC-V or is on an
embedded linux.

This project is inspired by the work of Rust's embedded_hal and follows many of
the same design goals.

libembeddedhal's design goals:

1. Serve as a foundation for building an ecosystem of platform agnostic drivers.
2. Must abstract away device specific details like registers and bitmaps.
3. Must be generic across devices such that any platform can be supported.
4. Must be minimal for boosting performance and reducing size costs.
5. Must be composable such that higher level drivers can build on top of these.
6. Be accessible through package mangers so that developers can easily pick and
   choose which drivers they want to use.

# 🪪 Library Badges

![uses](https://img.shields.io/badge/libembeddedhal-🔗%20uses-brightgreen)
![floats](https://img.shields.io/badge/libembeddedhal-⚠️%20floats-ff0000)
![throws](https://img.shields.io/badge/libembeddedhal-⚠️%20throws-ff0000)
![allocates](https://img.shields.io/badge/libembeddedhal-⚠️%20allocates-ff0000)
![safety-critical](https://img.shields.io/badge/libembeddedhal-🦺%20safety%20critical-yellow)

Each FOSS repo that implements libembeddedhal should include the following
badges in their root directory's README.md right below the title file if they
apply to the project's code. These should not reflect aspects of test code.

When searching for a library to use for your project, these badges can help you
decide if the project meets your requirements.

## 🔗 uses

All libraries that implement libembeddedhal interfaces should have this badge to
indicate library users/consumers.

## ⚠️ floats

If a library uses floating point arithmetic anywhere in its implementation.

## ⚠️ throws

If a library ever throws an exception anywhere in its code base.

## ⚠️ allocates

This badge is placed for a libraries that have the possibility to dynamically
allocates memory via `new`, `malloc`, `std::allocator` or a standard library
that uses any of the allocating functions.

## 🦺 safety critical

If a library follows completely the AUTOSAR C++20 guidelines.

# 🔨 Development Guides

All guides follow the [C++ Core
Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines).

## 📜 Coding Policies

Listed below are the policies that every libembeddedhal implementation must
follow to ensure consistent behavior, performance and size cost:

### Style

- Code shall follow libembeddedhal's `.clang-format` file, which uses the
  Mozilla C++ style format as a base with some adjustments.
- Code shall follow libembeddedhal's `.naming.style` file, which is very
  similar to the standard library naming convention:
  - CamelCase for template parameters.
  - CAP_CASE for macros.
  - lowercase snake_case for everything else.
  - prefix `p_` for function parameters.
  - prefix `m_` for private/protected class member.
- Refrain from variable names with abbreviations where it can be helped. `adc`
  `pwm` and `i2c` are extremely common so it is fine to leave them
  abbreviations. Most people know the abbreviations more than the words that
  make them up. But words `cnt` should be `count` and `cdl` and `cdh` should be
  written out as `clock_divider_low` and `clock_divider_high`. Registers do get
  a pass if they directly reflect the names in the data sheet which will make
  looking them up easier in the future.
- Use `#pragma once` as the include guard for headers.
- Every file must end with a newline character.
- Every line in a file must stay within a 80 character limit.
  - Exceptions to this rule are allowed. Use // NOLINT in these cases.
- Radix for bit manipulation:
  - Only use binary (`0b1000'0011`) or hex (`0x0FF0`) for bit manipulation.
  - Never use decimal or octal as this is harder to reasonable about for most
    programmers.
- Every public API must be documented with the doxygen style comments (CI will
  ensure that every public API is documented fully).
- Include the C++ header version of C headers such as `<cstdint>` vs
  `<stdint.h>`.

### Coding Restrictions

- Use the `libxbitset` library to perform bitwise operations operations.
- Only use macros if something cannot be done without using them. Usually macros
  can be replaced with constexpr or const variables or function calls. A case
  where macros are the only way is for BOOST_LEAF_CHECK() since there is no way
  to automatically generate the boiler plate for returning if a function returns
  and error in C++ and thus a macro is needed here to prevent possible mistakes
  in writing out the boilerplate.
- Only use preprocessor `#if` and the like if it is impossible to use
  `if constexpr` to achieve the same behavior.
- Never include `<iostream>` as it incurs an automatic 150kB space penalty even
  if the application never uses any part of `<iostream>`.
- Drivers should refrain from memory allocate as much as possible that includes
  using STL libraries that allocate such as `std::string` or `std::vector`.
- Logging within a library is prohibited for two reasons:
  - String formatting libraries may not be the same across libraries and an
    application including both will have to pay the space cost for two separate
    formatting libraries.
  - libembeddedhal libraries do not have the right to output to stdout/stderr,
    that is the role and responsibility of the application.
- Interfaces must follow the public API, private virtual method shown
  [here](http://www.gotw.ca/publications/mill18.htm).
- Inclusion of a C header file full of register map structures is not allowed as
  it would pollute the global namespace and tends to result in name collisions.



## Writing an interface

Guidelines for interfaces:

1. Follow the private virtual api pattern which looks like this:

```C++
class interface
{
 public:
  /**
   * ... API behavior documentation goes here ...
   */
  [[nodiscard]] boost::leaf::result<void> do_something() {
    return driver_do_something();
  }
 private:
  virtual boost::leaf::result<void> driver_do_something() = 0;
};
```

2. Outside of rule #1, do not include any non-virtual member functions in the
   interface.
3. For configuration data for which the bounds are not defined and are invariant
   the actual API use a configure function with this signature
   `boost::leaf::result<void> configure(const settings&)` where `settings` is an
   inner `struct` defined within the interface class's namespace. See
   `include/libembeddedhal/serial/serial.hpp` as an example already in use.
4. All members of a `settings` `struct` must be initialized with default values.
   These defaults should be values that most system can support.
5. All virtual member functions must have the following decorations:
   1. return types: `boost::leaf::result<T>` to order to allow error
      signaling to propagate, where T is the type you want to return.
   2. Marked as `noexcept`
   3. Marked as `[[nodsicard]]`

## Writing a Peripheral Driver (memory mapped I/O)

Follow along with the comments in the example C++ below to get an idea of how
to create a standard peripheral implementation. This is only meant for drivers
that use memory mapped I/O and do not require other drivers to operate. If
other device drivers are needed for your implementation, please
"Implementing a Device Driver".

Rules for peripheral driver implementations:

1. Keep the global namespace clean of any implementation details. This can be
   done by nesting all of the supporting code inside of the peripheral library's
   class. This includes memory maps, bit masks, const/constexpr values.
2. Do not depend on the data section being initialized when the constructor is
   executed. This means that non-zero initialized global variables are not
   allowed for drivers. This includes `static inline` variables nested within
   the class.
3. All drivers must be single phase initialized, meaning that that a driver must
   be usable after it has been constructed. Drivers should initialize any hard
   dependencies such as interrupt vector tables, or clock initializations should
   be done at this point in the code.
4. Expect that that the `.bss` section will be cleared to `0`
5. Expect that the order of constructors across compilation is effectively
   random.

## Writing a Device Drivers

(TODO)

# 📚 Libraries

## Processors

- [libarmcortex](https://github.com/libembeddedhal/libarmcortex): Drivers for
  the ARM Cortex M series of processors.
- [libriscvi32](https://github.com/libembeddedhal/libriscvi32): Coming soon.
  Drivers for 32-bit RISC-V processors

## Targets

- [liblpc40xx](https://github.com/libembeddedhal/liblpc40xx): Drivers the lpc40xx
  series of microcontrollers. This includes startup code, linker scripts, and
  peripheral drivers.
- [libstm32f1xx](https://github.com/libembeddedhal/libstm32f1xx): Coming soon.
  Drivers for atmega328 micro-controller

### Drivers

- [libesp8266](https://github.com/libembeddedhal/libesp8266): Drivers for the
  esp8266 micro-controller as well as drivers for the WiFi/Internet firmware AT
  client.
- [libmpu6050]((https://github.com/libembeddedhal/libmpu6050)): Coming soon.
  Accelerometer and gyroscope device. Requires an i2c driver.
- [libatmega328](https://github.com/libembeddedhal/libatmega328): Coming soon.
  Drivers for atmega328 micro-controller

# 👥 Contributing

## 🤚 Self Assigning to an Issue

If you find an issue you'd like to work on, simply type and submit a comment
with the phrase `.take` in it to get assigned by our github actions.

## 📤 Submitting a PR

Submitting a PR at anytime is fine, but code will not be reviewed until all of
the continuous integration steps have finished.
