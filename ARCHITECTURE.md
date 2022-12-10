# Architecture

The name libhal breaks down to the two words `lib` standing for library and
hal which stands for hardware abstraction layer.

libhal is a hardware abstraction layer library written in C++ and uses runtime
virtual polymorphism for its interfaces. It includes generic and common hardware
interfaces such as i2c, serial/uart, pwm, spi, can and much more. libhal does
not include functional drivers, but exists as an interface between actual
drivers and application logic.

The purpose of libhal is to make embedded software applications agnostic to the
devices they use and the targets they run on, just like how systems level
programming languages abstract away the specifics of machine code from the
programmer.

## Dependencies

### Boost.LEAF

Libhal utilizes Boost.LEAF for error handling. Boost.LEAF provides an experience
akin to a fusion of Rust `?` operator and exception handling with a mechanism
for ensuring that errors are transported to their destination handlers faster
than what is achievable with `std::expected` or C++ exceptions.

Boost.LEAF is aliased in the library from `boost::leaf::` to `hal::` for
brevity.

Almost all interface functions return a `result<T>` type as it cannot be known
if a particular driver is backed by an implementation that can fail.

### Standard library Instability C++20 and beyond

As of this time, libhal plans to utilize the latest features of C++ as they
are made available across GCC and Clang.

There is potential to use the SEMVER version number to specify when the C++
version jumps from, lets say, C++20 to C++23. We do not believe in holding
the library back to support clients that are not willing to upgrade their
software to use the latest compilers available.

## Library Structure

In general libhal is a header only library and thus all source code is in the
`/include/libhal` directory.

libhal source code consist of interfaces, interface specific utilities,
general utilities, and building blocks for interfaces like units and error
handling types.

Folders within `/include/libhal` represents interfaces and their utilities.
For example the `can` interface would exist within `/include/libhal/can`. The
can interface will exist in file `/include/libhal/can/interface.hpp`. Mocks,
and additional helper functions and types exist within this function.

Generic utilities and helper types exist within `/include/libhal/`.
