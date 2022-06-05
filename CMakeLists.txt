cmake_minimum_required(VERSION 3.1...3.21)

set(CMAKE_C_COMPILER "gcc-11")
set(CMAKE_CXX_COMPILER "g++-11")

project(libembeddedhal VERSION 0.0.1 LANGUAGES CXX)
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include
  include/${PROJECT_NAME}/internal/third_party/units/core/include/
  include/${PROJECT_NAME}/internal/third_party/units/systems/isq/include
  include/${PROJECT_NAME}/internal/third_party/units/systems/isq-iec80000/include
  include/${PROJECT_NAME}/internal/third_party/units/systems/isq-natural/include
  include/${PROJECT_NAME}/internal/third_party/units/systems/si/include
  include/${PROJECT_NAME}/internal/third_party/units/systems/si-cgs/include
  include/${PROJECT_NAME}/internal/third_party/units/systems/si-fps/include
  include/${PROJECT_NAME}/internal/third_party/units/systems/si-hep/include
  include/${PROJECT_NAME}/internal/third_party/units/systems/si-iau/include
  include/${PROJECT_NAME}/internal/third_party/units/systems/si-imperial/include
  include/${PROJECT_NAME}/internal/third_party/units/systems/si-international/include
  include/${PROJECT_NAME}/internal/third_party/units/systems/si-typographic/include
  include/${PROJECT_NAME}/internal/third_party/units/systems/si-uscs/include)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  PUBLIC_HEADER DESTINATION include)

find_package(ut)
find_package(libembeddedhal)
find_package(gsl-lite)

set(TEST_NAME unit_test)
set(CMAKE_BUILD_TYPE Debug)

add_executable(${TEST_NAME}
  tests/accelerometer/interface.test.cpp
  tests/can/interface.test.cpp
  tests/can/interface.test.cpp
  tests/pwm/interface.test.cpp
  tests/timer/interface.test.cpp
  tests/i2c/interface.test.cpp
  tests/compass/interface.test.cpp
  tests/gyroscope/interface.test.cpp
  tests/spi/interface.test.cpp
  tests/adc/interface.test.cpp
  tests/rtc/interface.test.cpp
  tests/temperature/interface.test.cpp
  tests/pixel_display/interface.test.cpp
  tests/text_display/interface.test.cpp
  tests/dac/interface.test.cpp
  tests/counter/interface.test.cpp
  tests/counter/interface.test.cpp
  tests/input_pin/interface.test.cpp
  tests/interrupt_pin/interface.test.cpp
  tests/output_pin/interface.test.cpp
  tests/serial/interface.test.cpp

  tests/i2c/util.test.cpp
  tests/spi/util.test.cpp
  tests/counter/util.test.cpp
  tests/serial/util.test.cpp
  tests/timer/util.test.cpp

  tests/motor/mock.test.cpp
  tests/pwm/mock.test.cpp
  tests/timer/mock.test.cpp
  tests/spi/mock.test.cpp
  tests/dac/mock.test.cpp
  tests/adc/mock.test.cpp

  tests/static_memory_resource.test.cpp
  tests/frequency.test.cpp
  tests/error.test.cpp
  tests/enum.test.cpp
  tests/percent.test.cpp
  tests/time.test.cpp
  tests/static_callable.test.cpp
  tests/testing.test.cpp
  tests/main.test.cpp
  tests/overflow_counter.test.cpp
  tests/units.test.cpp)

enable_testing()
add_test(NAME ${TEST_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMAND ${TEST_NAME})

target_include_directories(${TEST_NAME} PUBLIC tests)
target_compile_options(${TEST_NAME} PRIVATE -Werror -Wall -Wextra
  -Wno-unused-function -Wconversion)
set_target_properties(${TEST_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
target_compile_features(${TEST_NAME} PRIVATE cxx_std_20)
set_target_properties(${TEST_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(${TEST_NAME} PRIVATE ${PROJECT_NAME}
  boost::ut gsl::gsl-lite)
