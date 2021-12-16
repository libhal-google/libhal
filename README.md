<p align="center">
  <img height="150" src="docs/chip.svg">
</p>

# libembeddedhal

[![docs](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/docs.yml/badge.svg?branch=main)](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/docs.yml)
[![lint](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/lint.yml/badge.svg?branch=main)](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/lint.yml)
[![tests](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/tests.yml/badge.svg?branch=main)](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/tests.yml)

A collection of interfaces and abstractions for embedded peripherals and devices
using modern C++ best practices.

## 🏗️ WARNING: PROJECT IN DEVELOPMENT 🚧

## [📖 Full Documentation & Guides](https://sjsu-dev2.github.io/libembeddedhal)

## Development

### Installing Prerequisites

1. [Install Homebrew (Mac OS only)](https://brew.sh/)
1. [Git Download Page](https://git-scm.com/downloads)
1. [Python3 Download Page](https://www.python.org/downloads/)
1. Install g++:
    - Mac: `brew install gcc`
    - Ubuntu/Debian: `sudo apt install g++`
    - Windows: [install msys2](https://www.msys2.org/#installation)
1. Install CMake: `pip install cmake`
1. Conan: `pip install conan`

## Unit tests

The commands to build and run the tests are as follows from the root of the
repo:

```bash
conan create .
cd tests
CXX=g++-11 cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cd build
make
./bin/tests
```

### Explaining the steps:

1. First step is generating a conan package and installing it into your local conan
cache. This allows build systems like CMake to find and use the project like it
would a typical library.
1. Next we simply enter the test directory
1. For CMake:
  1. We ensure that the compiler used for building is is g++11, so we CXX=g++-11
  1. We set the build directory to `build` using the `-B` flag
  1. We set the location of the source to the current directory with `-S .`
  1. We set the build type to DEBUG mode to turn off optimizations. This is
     helpful for stepping through test code as well as ensuring that the test
     code works as intended without too much interference from the optimizer
     optimizing code out.
  1. Enter the build directory
  1. Run make to actually build the test executable
  1. Run the test executable

### Installing ARM toolchain

libembeddedhal is platform and architecture agnostic but if you do plan to build
application for ARM Cortex M class microcontrollers then you will want to follow
these steps:

- Ubuntu/Debian: `sudo apt install arm-none-eabi-gcc`
- Mac: `brew install --cask gcc-arm-embedded`
- Windows: Installer found here: [GNU Arm Embedded Toolchain Downloads](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
- Or you can download the prebuilt binaries for your platform and manually
  add them to your systems PATH variable, if you do not, or cannot install
  it globally.

## Developing on projects in the ecosystem
<details>
  <summary>Click to expand</summary>

### Cloning the projects

In order to develop for libembeddedhal and all of its implementations and
libraries, you will need to clone their repos manual (at least for the time
being).

NOTE: If you have a fork of a particular library you can swap out the git url to
point to your forked instance.


```bash
mkdir libembeddedhal-dev
cd libembeddedhal-dev

git clone https://github.com/SJSU-Dev2/liblpc40xx.git
git clone https://github.com/SJSU-Dev2/libesp8266.git
git clone https://github.com/SJSU-Dev2/libembeddedhal.git
git clone https://github.com/SJSU-Dev2/libarmcortex.git
git clone https://github.com/SJSU-Dev2/libxbitset.git
```

### Install libraries into local conan

```bash
conan create libxbitset
conan create libembeddedhal
conan create libarmcortex
conan create liblpc40xx
conan create libesp8266
```

### Running unit tests or cmake

In general what you do for build for cmake is to do the following at the root of
one of the repos:

```bash
cd tests
CXX=g++-11 cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cd build
make
./bin/tests
```

To explain the steps:

1. Make a build directory to hold of the build artifacts
2. Change into the build directory
3. Run `conan install` to install/download the libraries listed in the
conanfile.py. The "debug" build flags ensures that the build for unit tests and
the like are using debug level optimization. You can change to this to others if
you wish.
4. Run cmake in order to generate the makefiles needed to build the application
5. Run make to actually build the code.

### Building applications

For now, see https://github.com/SJSU-Dev2/starter-embeddedhal. The steps should
be the same as "Running unit tests or cmake" and tinker with the CMakeLists.txt
file in order modify the code but

### Making changes to a library avaible

Each time you make a change to a library, you need to run `conan create` on its
directory.

</details>
