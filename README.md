<p align="center">
  <img height="150" src="docs/chip.svg">
</p>

# libembeddedhal

[![docs](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/docs.yml/badge.svg?branch=main)](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/docs.yml)
[![lint](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/lint.yml/badge.svg?branch=main)](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/lint.yml)
[![tests](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/tests.yml/badge.svg?branch=main)](https://github.com/SJSU-Dev2/libembeddedhal/actions/workflows/tests.yml)

A collection of interfaces and abstractions for embedded peripherals and devices
using modern C++

## 🏗️ WARNING IN DEVELOPMENT 🚧

## Development

### Prerequisites

1. Python3
2. CMake
3. Conan
4. Target g++ with C++20 support

#### Installing Prerequisites

- **Python3**: See https://www.python.org/downloads/
- **CMake**: `pip install cmake`
- **Conan**: `pip install conan`
- **Target (ARM) g++**: See https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
    - On Ubuntu/Debian simply run `sudo apt install arm-none-eabi-gcc`

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
mkdir build
cd build
conan install .. -s build_type=Debug
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
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
