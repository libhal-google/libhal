<p align="center">
  <img height="150" src="docs/chip.svg">
</p>

# libembeddedhal

[![docs](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/docs.yml/badge.svg?branch=main)](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/docs.yml)
[![lint](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/lint.yml/badge.svg?branch=main)](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/lint.yml)
[![tests](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/tests.yml/badge.svg?branch=main)](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/tests.yml)

## 🏗️ WARNING: PROJECT IN DEVELOPMENT 🚧

| [Overview](#overview)
| [Install](#install)
| [Motivation](#motivation)
| [Policies](#policies)
| [User Guides](#user-guides)
| [Libraries](#libraries)
| [Developer Guides](#developer-guides)
| [FAQ](#faq)
|

### [⚙️ Software APIs](https://sjsu-dev2.github.io/libembeddedhal/)

<details open>
<summary> 📥 Install </summary>
<hr/>

Install from conan:

```bash
conan install libembeddedhal
```

Installing from source locally:

```bash
git clone https://github.com/SJSU-Dev2/libembeddedhal.git
conan create libembeddedhal
```

</details>

<details>
<summary> ℹ️ Overview </summary>
<hr/>

libembeddedhal is a collection of interfaces and abstractions for embedded
system peripherals and devices using modern C++ best practices. The goal is to
make writing embedded systems software & applications easy for everyone.

- Header only
- Dependencies:
  - Boost.LEAF for error handling
  - C++20 and above (currently only supports g++-10 and above)
- Target/platform agnostic (ARM Cortex, STM32, AVR, RPI, embedded linux, etc)
- Designed to be modular, dynamic, composable, lightweight
- Throwing exceptions out of drivers is strictly forbidden
- Dynamically allocating memory is forbidden (rare exceptions may exist for some
  libraries)
- Follows C++ Core Guidelines as much as possible
- Nearly no use of macros (currently only 1 macro)
- Customizable using tweak header files (not through macros)

</details>

<details>
<summary> 💡 Motivation </summary>
<hr/>

The world of embedded systems is written almost entirely in C and C++. More and
more the embedded world moves away from C and towards C++. This has to do with
the many benefits of C++ such as type safety, compile time features,
meta-programming, multiple programming paradigms which, if use correctly can
result in smaller and higher performance code than in C.

But a problem for embedded software in C++, as well as in C, is that there isn't
a consistent and common API for embedded libraries. Looking around, you will
find that each vendor of embedded products has their own set of libraries and
tools for their specific products. If you write a driver on top of their
libraries, you will find that your code will only work for that specific
platform, using that specific API and in some cases using a specific toolchain.
You as the developer are locked in to this one specific setup. And if you move
to another platform, you must do the work of rewriting all of your code again.

libembeddedhal seeks to solve this issue by creating a set of generic interfaces
for embedded system concepts such as serial communication (UART), analog to
digital conversion (ADC), inertial measurement units (IMU), pulse width
modulation (PWM) and much more. The advantage of building a system on top of
libembeddedhal is that higher level drivers can be used with any target platform
whether it is an stm32, an nxp micro controller, runs on RISC-V or is on an
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

</details>

<details>
<summary> 🎛️ Customization </summary>
<hr/>

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

</details>

<details>
<summary> 📜 Coding Policies </summary>
<hr/>

Listed below are the policies that every libembeddedhal implementation must
follow to ensure consistent behavior, performance and size cost:

<details>
<summary>Style</summary>

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

</details>

<details>
<summary>Coding Restrictions</summary>

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

</details>
</details>

<details>
<summary> 📖 Guides </summary>
<hr/>

All guides follow the [C++ Core
Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines).

<details>
<summary>Creating a new interface</summary>

Guidelines for interfaces:

1. Do not include any non-virtual member functions in the interface
2. For configuration data for which the bounds are not defined and are invariant
   the actual API use a configure function with this signature
   `boost::leaf::result<void> configure(const settings&)` where `settings` is an
   inner `struct` defined within the interface class's namespace. See
   `include/libembeddedhal/serial/serial.hpp` as an example already in use.
3. All members of a `settings` `struct` must be initialized with default values
   that most systems can be expected to achieve.
4. All virtual member functions must have the following decorations:
    1. return types: `boost::leaf::result<T>` to order to allow error
       signaling to propagate, where T is the type you want to return.
    2. Marked as `noexcept`
    3. Marked as `[[nodsicard]]`

</details>

<details>
<summary>Implementing a Peripheral Driver</summary>

Follow along with the comments in the example C++ below to get an idea of how
to create a standard peripheral implementation.

```C++
/// Since this is a header only driver you'll need pragma once
#pragma once

/// Be sure to include the adc interface you plan to implement
#include <libembeddedhal/adc/adc.hpp>
/// Very likely you'll need to perform bit manipulation so its usually safe to
/// include this as well. Remove this if you do not end up performing any bit
/// manipulation.
#include <libxbitset/bitset.hpp>

/// Make sure to put your driver within a namespace. Do not pollute the global
/// namespace with your driver and any other variables, functions or objects.
/// Prefer the namespace name "embed::<insert_platform_name_here>".
namespace embed::lpc40xx {
/// Give the class a simple name and inherit the desired interface.
class adc : public embed::adc
{
public:
  /// Channel specific information.
  /// This name can be "port", "bus" or anything else that makes sense.
  struct channel
  {
    // Add fields necessary for the driver to work ...
  };

  /// Structure acting as a namespace to contain bit mask objects. In this
  /// example, these bit masks are used to manipulate the ADC's control
  /// register.
  struct control_register
  {
    /// Add bit masks here
  };

  /// Create a structure representing your register map. Usually vendors for an
  /// MCU provide a C header file with all of the registers. Use that as a
  /// reference for making this register map. Don't forget to make each entry
  /// volatile so that the values are not cached when written to or read from.
  /// If there is only 1 register map for this class, simply call it "reg_t".
  /// Otherwise give it a name that makes sense for it and the others.
  struct reg_t
  {
    /// Add register map fields here
  };

  /// Add a static public member for getting the register map.
  ///
  /// If the code is running on the correct platform, then return the address
  /// of the peripheral.
  ///
  /// If the code is being run in a unit test, or doesn't match the intended
  /// platform, it returns a reference dummy version of the register map.
  ///
  /// If there are multiple peripherals that share the same peripheral register
  /// map, then this function should take int as an index for which peripheral
  /// to return.
  static reg_t* reg()
  {
    if constexpr (embed::is_platform("lpc40")) {
      static constexpr intptr_t lpc_apb0_base = 0x40000000UL;
      static constexpr intptr_t lpc_adc_addr = lpc_apb0_base + 0x34000;
      return reinterpret_cast<reg_t*>(lpc_adc_addr);
    } else {
      static reg_t dummy{};
      return &dummy;
    }
  }

  /// Create a constructor that accepts the channel details as well as any
  /// settings
  adc(channel p_channel) noexcept
    : m_channel(p_channel)
  {
    // Step 1. Turn on the peripheral.
    //         For some devices that means flipping a power bit.
    //         For others it means enabling a bit that allows a clock through.
    //         It could be something else for your system.
    //
    // Step 2. Setup any pins if that means anything for the peripheral.
    //
    // Step 3. Setup any clocks and pre-scalars and dividers for the peripheral
    //         as well as any other peripheral setup
    //
    // Step 4. If the constructor takes any configuration settings pass these
    //         to the driver_configure() function here.
  }

private:
  // Declare implementation of virtual functions here (don't forget "override")
  boost::leaf::result<percent> driver_read() noexcept override;
};

// Create a `get_<insert peripheral name here>` function that takes a template
// parameter for which peripheral this is.
template<int Channel>
adc& get_adc()
{
  // Use `if constexpr` in order to ensure that only one of these blocks exists
  // for each possible instance of "Channel"
  if constexpr (Channel == 0) {
    constexpr adc::channel channel0 = {
      .port = 0,
      .pin = 23,
      .index = 0,
      .pin_function = 0b011,
    };
    static adc adc_channel0(channel0);
    return adc_channel0;
  } else if constexpr (Channel == 1) {
    // Same as Channel 0 but with the settings for channel 1
  } else if constexpr (Channel == 2) {
    // Same as Channel 0 but with the settings for channel 2
  } else if constexpr (Channel == 3) {
    // Same as Channel 0 but with the settings for channel 3
  } else if constexpr (Channel == 4) {
    // Same as Channel 0 but with the settings for channel 4
  } else if constexpr (Channel == 5) {
    // Same as Channel 0 but with the settings for channel 5
  } else if constexpr (Channel == 6) {
    // Same as Channel 0 but with the settings for channel 6
  } else if constexpr (Channel == 7) {
    // Same as Channel 0 but with the settings for channel 7
  } else {
    // Place a static assert here to generate a compiler error for the user if
    // the accidentally used a channel outside of the bounds of the ADC driver.
    static_assert(error::invalid_option<Channel>,
                  "\n\n"
                  "LPC40xx Compile Time Error:\n"
                  "    LPC40xx only supports ADC channels from 0 to 7. \n"
                  "\n");
    return get_adc<0>();
  }
}

// Implement the driver outside of the class in an inline function.
inline boost::leaf::result<percent> adc::driver_read()
{
  // implementation here ...
  // Don't forget the return value ...
}
}  // namespace embed::lpc40xx
```

</details>

<details>
<summary>Device Drivers</summary>

TDB

</details>
</details>

<details>
<summary>📚 Libraries</summary>

- [libarmcortex](https://github.com/SJSU-Dev2/libarmcortex): drivers for the ARM
  Cortex M series of processors.
- [liblpc40xx](https://github.com/SJSU-Dev2/liblpc40xx): drivers the lpc40xx
  series of microcontrollers. This includes startup code, linker scripts, and
  peripheral drivers.
- [libesp8266](https://github.com/SJSU-Dev2/libesp8266): WiFi card driver with
  TCP/IP communication as well. Requires a serial driver
- libmpu6050: coming soon. Accelerometer and gyroscope device. Requires an i2c
  driver.
- libstm32f1xx: coming soon. Drivers for atmega328 microntrollers
- libatmega328: coming soon. Drivers for atmega328 microntrollers
- libriscvi32: coming soon. Drivers for 32-bit RISC-V processors

</details>

<details>
<summary>👥 Contributing</summary>

TBD

</details>
