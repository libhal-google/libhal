#!/bin/bash

# Set environment such that the script ends if any command fails
set -e

# Move to the location of this script
script_path="$(dirname "${BASH_SOURCE[0]}")"
cd $script_path

# Create and locally install our conan package
conan create ..

# Create, if not present, the "build" directory and move into it
mkdir -p build
cd build

# Install conan packages (Boost.UT)
conan install .. -s build_type=Debug -r=libhal-trunk --update
# Generate build files
cmake ..
# Build program
make -j unit_test

# Execute unit tests
./unit_test

# ... PROFIT!
