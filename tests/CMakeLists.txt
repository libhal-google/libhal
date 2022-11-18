cmake_minimum_required(VERSION 3.15)

project(unit_test VERSION 0.0.1 LANGUAGES CXX)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # require at least gcc 11
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.0)
    message(FATAL_ERROR "GCC version must be at least 11!")
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # require at least clang 13
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13)
    message(FATAL_ERROR "Clang version must be at least 13!")
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  # require at least clang 13
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13)
    message(FATAL_ERROR "Clang version must be at least 13!")
  endif()
else()
  message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC, detected ${CMAKE_CXX_COMPILER_ID}.")
endif()

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_BUILD_TYPE "Debug")

find_package(ut REQUIRED CONFIG)
find_package(libhal REQUIRED CONFIG)

add_executable(${PROJECT_NAME}
  can/interface.test.cpp
  pwm/interface.test.cpp
  timer/interface.test.cpp
  i2c/interface.test.cpp
  spi/interface.test.cpp
  adc/interface.test.cpp
  dac/interface.test.cpp
  input_pin/interface.test.cpp
  interrupt_pin/interface.test.cpp
  output_pin/interface.test.cpp
  serial/interface.test.cpp
  servo/interface.test.cpp
  steady_clock/interface.test.cpp
  motor/interface.test.cpp

  i2c/util.test.cpp
  spi/util.test.cpp
  serial/util.test.cpp
  steady_clock/util.test.cpp

  serial/coroutine.test.cpp

  motor/mock.test.cpp
  pwm/mock.test.cpp
  timer/mock.test.cpp
  spi/mock.test.cpp
  dac/mock.test.cpp
  adc/mock.test.cpp
  output_pin/mock.test.cpp
  input_pin/mock.test.cpp
  interrupt_pin/mock.test.cpp
  servo/mock.test.cpp
  steady_clock/mock.test.cpp
  can/mock.test.cpp

  servo/rc.test.cpp
  i2c/minimum_speed.test.cpp
  can/router.test.cpp

  as_bytes.test.cpp
  enum.test.cpp
  timeout.test.cpp
  static_list.test.cpp
  static_callable.test.cpp
  testing.test.cpp
  main.test.cpp
  move_interceptor.test.cpp
  overflow_counter.test.cpp
  math.test.cpp
  error.test.cpp
  streams.test.cpp
  map.test.cpp
  units.test.cpp
  callback_recorder.test.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC . ../include)
target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra
  -Wno-unused-function -Wconversion -Wno-psabi)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::ut libhal::libhal)
