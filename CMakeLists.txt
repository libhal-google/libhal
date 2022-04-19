cmake_minimum_required(VERSION 3.1...3.21)

set(CMAKE_C_COMPILER "gcc-11")
set(CMAKE_CXX_COMPILER "g++-11")

project(libembeddedhal VERSION 0.0.1 LANGUAGES CXX)
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  PUBLIC_HEADER DESTINATION include)

find_package(ut)
find_package(libembeddedhal)

set(TEST_NAME unit_test)
set(CMAKE_BUILD_TYPE Debug)

add_executable(${TEST_NAME} tests/can/can.test.cpp
  tests/can/network.test.cpp
  tests/error.test.cpp
  tests/pwm/pwm_mock.test.cpp
  tests/pwm/pwm.test.cpp
  tests/enum.test.cpp
  tests/timer/timer.test.cpp
  tests/i2c/i2c_util.test.cpp
  tests/i2c/i2c.test.cpp
  tests/percent.test.cpp
  tests/time.test.cpp
  tests/static_callable.test.cpp
  tests/imu/accelerometer.test.cpp
  tests/imu/gyroscope.test.cpp
  tests/imu/compass.test.cpp
  tests/spi/spi.test.cpp
  tests/spi/spi_util.test.cpp
  tests/adc/adc.test.cpp
  tests/frequency.test.cpp
  tests/rtc/rtc.test.cpp
  tests/main.test.cpp
  tests/testing.test.cpp
  tests/display/pixel_display.test.cpp
  tests/display/text_display.test.cpp
  tests/dac/dac_stub.test.cpp
  tests/dac/dac.test.cpp
  tests/counter/counter.test.cpp
  tests/counter/counter_utility.test.cpp
  tests/counter/uptime_counter.test.cpp
  tests/overflow_counter.test.cpp
  tests/gpio/gpio.test.cpp
  tests/static_memory_resource.test.cpp
  tests/serial/serial_stub.test.cpp
  tests/serial/serial_util.test.cpp
  tests/serial/serial.test.cpp)

enable_testing()
add_test(NAME ${TEST_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMAND ${TEST_NAME})

target_include_directories(${TEST_NAME} PUBLIC tests)
target_compile_options(${TEST_NAME} PRIVATE -Werror -Wall -Wextra
  -Wno-unused-function -Wconversion -g)
set_target_properties(${TEST_NAME} PROPERTIES CXX_VISIBILITY_PRESET default)
target_compile_features(${TEST_NAME} PRIVATE cxx_std_20)
set_target_properties(${TEST_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(${TEST_NAME} PRIVATE boost::ut ${PROJECT_NAME})
