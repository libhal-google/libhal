# How to install prerequisites for libhal

NOTE: that these prerequisites aren't the only way one can use and install
libhal, but these are most supported and recommended way of using libhal.

## Prerequisites

- `make`: latest available version installed via "build essentials" package
- `cmake`: 3.15 or above
- `python`: 3.10 or above
- `conan`: 1.51.3 or above (conan 2.0.0+ not currently supported)
- `gcc`: version 11.3.0 or above (for compiling tests and code in general)

## Installing on Ubuntu 22.04:

Python 3.10 is default installed.

### 1. Install GCC

Install GCC and build essentials.

```
sudo apt update
sudo apt upgrade
sudo apt install -y build-essential g++-11
```

### 2. Installing cmake

```
pip install cmake
```

### 3. Installing conan

```
pip install conan
```

## Installing on MacOS X:

### 1. Install Homebrew

```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### 2. Install latest version of Python 3.x

Install python (will default to the latest Python3 version).

```
brew install python
```

### 3. Install GCC

```
brew install gcc
```

### 4. Install cmake

```
pip install cmake
```

### 5. Install conan

```
pip install conan
```

## Installation steps for Windows

### 1. Installing python

Download and run the python installer from this web page
https://www.python.org/downloads/windows/.

### 2. Installing MSYS2

Follow the instructions found here: https://www.msys2.org/

### 3. Install GCC and build essentials

Within the MSYS2 terminal run the following command to install GCC and
build essentials.

```
pacman -S base-devel mingw-w64-x86_64-gcc
```

### 4. Updating the Environment Paths

Open `Windows Powershell` as `Administrator` (right click on it to see the
option) and execute the following command. This will add gcc and the build
essentials to your systems environment paths.

```PowerShell
[Environment]::SetEnvironmentVariable(
    "Path",
    [Environment]::GetEnvironmentVariable("Path", [EnvironmentVariableTarget]::Machine) +
    ";C:\msys64\mingw64\bin;C:\msys64\mingw32\bin;C:\msys64\usr\bin",
    [EnvironmentVariableTarget]::Machine)
```

After this, close powershell and reopen it to check if it worked. Run
`g++ --version` and you should get a message with version information in it.

### 5. Installing cmake

```
pip install cmake
```

### 6. Installing conan

```
pip install conan
```

## Troubleshooting

### `settings.compiler` not found error

First check that `g++ --version` runs and that the compiler version is `11` or
above. If not, please install the appropriate compiler. If g++ is found then
run the following command to get conan to autodetect your default compiler on
your system.

```
conan profile default --detect --force
```
