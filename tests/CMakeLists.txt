cmake_minimum_required(VERSION 3.1...3.21)

set(CMAKE_C_COMPILER "gcc-11")
set(CMAKE_CXX_COMPILER "g++-11")

project(libhal_tests VERSION 0.0.1 LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(ut)

set(TEST_NAME unit_test)
set(CMAKE_BUILD_TYPE Debug)
set(LIST_OF_SOURCES
  can/interface.test.cpp
  pwm/interface.test.cpp
  timer/interface.test.cpp
  i2c/interface.test.cpp
  spi/interface.test.cpp
  adc/interface.test.cpp
  dac/interface.test.cpp
  counter/interface.test.cpp
  input_pin/interface.test.cpp
  interrupt_pin/interface.test.cpp
  output_pin/interface.test.cpp
  serial/interface.test.cpp
  servo/interface.test.cpp

  i2c/util.test.cpp
  spi/util.test.cpp
  counter/util.test.cpp
  serial/util.test.cpp

  motor/mock.test.cpp
  pwm/mock.test.cpp
  timer/mock.test.cpp
  spi/mock.test.cpp
  dac/mock.test.cpp
  adc/mock.test.cpp
  output_pin/mock.test.cpp
  input_pin/mock.test.cpp
  counter/mock.test.cpp
  interrupt_pin/mock.test.cpp
  servo/mock.test.cpp

  servo/rc.test.cpp
  i2c/minimum_speed.test.cpp
  counter/uptime_counter.test.cpp

  static_memory_resource.test.cpp
  enum.test.cpp
  percentage.test.cpp
  timeout.test.cpp
  static_callable.test.cpp
  testing.test.cpp
  main.test.cpp
  overflow_counter.test.cpp
  math.test.cpp
  error.test.cpp
  map.test.cpp
  units.test.cpp)

set(WARNING_FLAGS -Werror -Wall -Wextra -Wno-unused-function -Wconversion
  -Wno-psabi)

add_executable(${TEST_NAME}_double ${LIST_OF_SOURCES})
target_include_directories(${TEST_NAME}_double PUBLIC
  . configs/double ../include)
target_compile_options(${TEST_NAME}_double PRIVATE ${WARNING_FLAGS})
set_target_properties(${TEST_NAME}_double PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_features(${TEST_NAME}_double PRIVATE cxx_std_20)
set_target_properties(${TEST_NAME}_double PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(${TEST_NAME}_double PRIVATE boost::ut)

add_executable(${TEST_NAME}_float ${LIST_OF_SOURCES})
target_include_directories(${TEST_NAME}_float PUBLIC
  . configs/float ../include)
target_compile_options(${TEST_NAME}_float PRIVATE ${WARNING_FLAGS})
set_target_properties(${TEST_NAME}_float PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_features(${TEST_NAME}_float PRIVATE cxx_std_20)
set_target_properties(${TEST_NAME}_float PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(${TEST_NAME}_float PRIVATE boost::ut)
