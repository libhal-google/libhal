cmake_minimum_required(VERSION 3.1...3.21)

project(test VERSION 0.0.1 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS
  "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD
    "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
    "${CMAKE_BINARY_DIR}/conan.cmake"
    EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
    TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(REQUIRES boost-ext-ut/1.1.8
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

add_executable(${PROJECT_NAME}
    can/can.test.cpp
    can/network.test.cpp
    pwm/pwm.test.cpp
    enum.test.cpp
    driver.test.cpp
    timer/timer.test.cpp
    i2c/i2c.test.cpp
    i2c/i2c_util.test.cpp
    time.test.cpp
    overflow_counter.test.cpp
    static_callable.test.cpp
    imu/accelerometer.test.cpp
    imu/gyroscope.test.cpp
    imu/compass.test.cpp
    spi/spi.test.cpp
    spi/spi_util.test.cpp
    adc/adc.test.cpp
    frequency.test.cpp
    rtc/rtc.test.cpp
    main.test.cpp
    display/pixel_display.test.cpp
    display/text_display.test.cpp
    dac/dac.test.cpp
    counter/counter.test.cpp
    counter/counter_utility.test.cpp
    percent.test.cpp
    gpio/gpio.test.cpp
    static_memory_resource.test.cpp
    serial/serial.test.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>)
target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET default)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(${PROJECT_NAME} PRIVATE
    CONAN_PKG::boost-ext-ut)
