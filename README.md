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
1. [Install Git](https://git-scm.com/downloads)
1. [Install Python3](https://www.python.org/downloads/)
1. Install g++-11:
    - Mac: `brew install gcc@11`
    - Ubuntu/Debian: `sudo apt install g++-11`
    - Windows: [install msys2](https://www.msys2.org/#installation)
1. Install CMake: `pip install cmake`
1. Conan: `pip install conan`

## Unit tests

Unit tests are built and executed as a part of the packaging system. To run unit
tests you can simply execute this command:

```bash
conan create .
```

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

### Building applications

For now, see https://github.com/SJSU-Dev2/starter-embeddedhal. The steps should
be the same as "Running unit tests or cmake" and tinker with the CMakeLists.txt
file in order modify the code but

### Making changes to a library available

Each time you make a change to a library, you need to run `conan create` on its
directory in order to get those changes across other libraries.

</details>
