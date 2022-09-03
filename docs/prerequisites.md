# How to install prerequisites for libhal

NOTE: that these prerequisites aren't the only way one can use and install
libhal, but these are most supported and recommended way of using libhal.

## Prerequisites

- `python`: 3.10 or above
- `cmake`: 3.15 or above
- `conan`: 1.51.3 or above (conan 2.0.0+ not currently supported)
- `g++`: version 11.3.0 or above (for compiling tests and code in general)

If you already have these installed you can skip this step.

## Installing python3

### Ubuntu 22.04 and above

Python 3.10 already installed, nothing to do here

### MacOS X

1. Install brew
  - Run this command:

```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

2. Install Python3

```
brew install python
```

### Windows

Download and run the installer which can be found here:
https://www.python.org/downloads/windows/

## Installing cmake

### For all systems

Execute the following command

```
pip install cmake
```

## Installing conan

```
pip install conan
```

## Installing g++-11 or above

### Ubuntu 22.04

```
sudo apt update
sudo apt upgrade
sudo apt install build-essential
sudo apt install -y g++-11
```

### MacOSX

```
brew install gcc@11
```

### Windows

NOTE: This section is not complete and a complete guide to installing g++-11
is not currently available. Still investigating the simplest way to achieve
this.

TODO(#429): Flesh out this section out

Download g++-11.3.0 from: https://winlibs.com/ and unzip it.
