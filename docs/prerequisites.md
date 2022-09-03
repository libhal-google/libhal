# How to install prerequisites for libhal

NOTE: that these prerequisites aren't the only way one can use and install
libhal, but these are most supported and recommended way of using libhal.

## Prerequisites

- `python`: 3.10 or above
- `cmake`: 3.15 or above
- `conan`: 1.51.3 or above (conan 2.0.0+ not currently supported)
- `g++`: version 11.3.0 or above (for compiling tests and code in general)

If you already have these installed you can skip this step.

## Installing on Ubuntu 22.04

NOTE: Python 3.10 already installed

```
pip install cmake
```

```
pip install conan
```

```
sudo apt update
sudo apt upgrade
sudo apt install build-essential
sudo apt install -y g++-11
```

### Installing on MacOS X

Open up a terminal and execute these commands one by one:

```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

```
brew install python
```

```
brew install gcc@11
```

```
pip install cmake
```

```
pip install conan
```

## Installation steps for Windows

### Installing python

Python Installer here: https://www.python.org/downloads/windows/

### Installing conan + cmake

```
pip install cmake
```

```
pip install conan
```

### Installing GCC 11

NOTE: This section is not complete and a complete guide to installing g++-11
is not currently available. Still investigating the simplest way to achieve
this.

TODO(#429): Flesh out this section out

Download g++-11.3.0 from: https://winlibs.com/ and unzip it.
