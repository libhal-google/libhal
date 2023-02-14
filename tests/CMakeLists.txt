cmake_minimum_required(VERSION 3.15)

project(unit_test VERSION 0.0.1 LANGUAGES CXX)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # require at least gcc 11
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.0)
    message(FATAL_ERROR "GCC version must be at least 11!")
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # require at least clang 14
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14)
    message(FATAL_ERROR "Clang version must be at least 14!")
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  # require at least clang 14
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14)
    message(FATAL_ERROR "Clang version must be at least 14!")
  endif()
else()
  message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC, detected ${CMAKE_CXX_COMPILER_ID}.")
endif()

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_BUILD_TYPE "Debug")

find_package(ut REQUIRED CONFIG)
find_package(tl-function-ref REQUIRED CONFIG)

add_executable(${PROJECT_NAME}
  can.test.cpp
  pwm.test.cpp
  timer.test.cpp
  i2c.test.cpp
  spi.test.cpp
  adc.test.cpp
  dac.test.cpp
  input_pin.test.cpp
  interrupt_pin.test.cpp
  output_pin.test.cpp
  serial.test.cpp
  socket.test.cpp
  steady_clock.test.cpp
  motor.test.cpp
  timeout.test.cpp
  error.test.cpp
  accelerometer.test.cpp
  distance_sensor.test.cpp
  gyroscope.test.cpp
  magnetometer.test.cpp
  rotation_sensor.test.cpp
  temperature_sensor.test.cpp
  main.test.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC . ../include)
target_compile_options(${PROJECT_NAME} PRIVATE
  --coverage
  -fprofile-arcs
  -ftest-coverage
  -fsanitize=address
  -Werror
  -Wall
  -Wextra
  -Wshadow
  -Wnon-virtual-dtor
  -Wno-gnu-statement-expression
  -pedantic)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_link_options(${PROJECT_NAME} PRIVATE
  --coverage
  -fprofile-arcs
  -ftest-coverage
  -fsanitize=address)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::ut tl::function-ref)

add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  -P ${CMAKE_SOURCE_DIR}/remove_gcda.cmake
  COMMENT "Deleting Previous Coverage Files..."
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
