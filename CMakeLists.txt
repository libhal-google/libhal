# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.1...3.21)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(libembeddedhal VERSION 0.0.1 LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# If you set any CMAKE_ variables, that can go here.
# (But usually don't do this, except maybe for C++ standard)

# Find packages go here.

# You should usually split this into folders, but this is a simple example

# This is a "default" library, and will match the *** variable setting.
# Other common choices are STATIC, SHARED, and MODULE
# Including header files here helps IDEs but is not required.
# Output libname matches target name, with the usual extensions on your system
add_library(libembeddedhal INTERFACE)

target_include_directories(libembeddedhal
                           INTERFACE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>)

# Link each target with other targets or add options, etc.
target_compile_features(libembeddedhal INTERFACE cxx_std_20)
set_target_properties(libembeddedhal PROPERTIES CXX_EXTENSIONS OFF)

# Adding something we can run - Output name matches target name
add_executable(libembeddedhal.test.exe libembeddedhal/full_scale.test.cpp)

# Make sure you link your targets with this command. It can also link libraries
# and even flags, so linking a target that does not exist will not give a
# configure-time error.
target_link_libraries(libembeddedhal.test.exe
                      PRIVATE
                      libembeddedhal
                      CONAN_PKG::boost-ext-ut)

set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(TARGETS libembeddedhal
       NAMESPACE libembeddedhal::
       FILE "${CMAKE_CURRENT_BINARY_DIR}/libembeddedhal-config.cmake")
export(PACKAGE libembeddedhal)
