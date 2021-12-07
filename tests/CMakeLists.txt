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

conan_cmake_run(REQUIRES libembeddedhal/0.0.1
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

conan_cmake_run(REQUIRES boost-ext-ut/1.1.8
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

add_executable(${PROJECT_NAME}
    context.test.cpp
    driver.test.cpp
    enum.test.cpp
    full_scale.test.cpp
    static_callable.test.cpp
    static_memory_resource.test.cpp
    time.test.cpp
    main.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(${PROJECT_NAME} PRIVATE -DPLATFORM=test)
target_link_libraries(${PROJECT_NAME} PRIVATE
    CONAN_PKG::libembeddedhal
    CONAN_PKG::boost-ext-ut)
