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

## Overview

libembeddedhal is a collection of interfaces and abstractions for embedded
system peripherals and devices using modern C++ best practices. The goal is to
make writing embedded systems software & applications easy for everyone.

- Header only
- Dependencies:
  - Boost.LEAF for error handling
  - C++20 and above (currently only supports g++-10 and above)
- Designed to be modular, dynamic, and composable
- Designed to support any embedded target
- Designed to be lightweight
- Designed to be simple and minimalist and support majority of use cases
- Nearly no use of macros (currently only 1 macro)
- Implementation policies
  - Throwing exceptions out of drivers is strictly forbidden
  - Refrain from dynamically allocating memory
  - Refrain from using the FPU
  - And more! See list of [implementation policies](#impl-policy)
- Customizable using tweak header files (not through macros)

## Install

```bash
conan install libembeddedhal
```

### Installing from source

```bash
git clone https://github.com/SJSU-Dev2/libembeddedhal.git
conan create libembeddedhal
```

NOTE: this will also run unit tests as well.

## Motivation

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

### Design Goals

1. Serve as a foundation for building an ecosystem of platform agnostic drivers.
2. Must abstract away device specific details like registers and bitmaps.
3. Must be generic across devices such that any platform can be supported.
4. Must be minimal for boosting performance and reducing size costs.
5. Must be composable such that higher level drivers can build on top of these.
6. Be accessible through package mangers so that developers can easily pick and
   choose which drivers they want to use.

## Policies

Listed below are the policies that every libembeddedhal implementation must
follow to ensure consistent behavior, performance and size cost:

### Style

<details>
<summary>Click to expand</summary>

- Code shall follow libembeddedhal's `.clang-format` file, which uses the
  Mozilla C++ style format as a base with some adjustments.
- Code shall follow libembeddedhal's `.naming.style` file, which is very
  similar to the standard library naming convention as well as
  [Jason Turner's style](https://lefticus.gitbooks.io/cpp-best-practices/content/03-Style.html).
  - CamelCase for template parameters.
  - CAP_CASE for macros.
  - snake_case for everything else.
  - prefix `p_` for function parameters.
  - prefix `m_` for private/protected class member.
- Refrain from variable names with abbreviations where it can be helped. `adc`
  `pwm` and `i2c` are extremely common so it is fine to leave them abbreviations
  but `cntr` should be `counter` and `cdl` and `cdh` should be written out as
  `clock_divider_low` and `clock_divider_high`. Registers do get a pass if they
  directly reflect the names in the data sheet which will make looking them up
  easier in the future.
- Use `#pragma once` as your include guard for headers.
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

### Coding Restrictions

<details>
<summary>Click to expand</summary>

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

</details>

## Guides

### Classification of Drivers

<details>
<summary>Click to expand</summary>

TDB

</details>

### Writing Peripheral Drivers

<details>
<summary>Click to expand</summary>

TDB

</details>

### Writing Device Drivers

<details>
<summary>Click to expand</summary>

TDB

</details>

## Libraries

<details>
<summary>Click to expand</summary>

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
- libatmega328: comming soon. Drivers for atmega328 microntrollers
- libriscvi32: coming soon. Drivers for 32-bit RISC-V processors

</details>

## Developing on projects in the ecosystem
<details>
<summary>Click to expand</summary>
</details>
