cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE INTERNAL "Default to Debug Build")
endif(NOT DEFINED CMAKE_BUILD_TYPE)

set(CMAKE_TOOLCHAIN_FILE conan_toolchain.cmake)

# Set project name to demos
project(demos VERSION 0.0.1 LANGUAGES CXX)

find_package(libhal REQUIRED CONFIG)

message(STATUS "Generating Demo for \"${PROJECT_NAME}")
add_executable(${PROJECT_NAME}
    main.cpp
    newlib.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} PRIVATE libhal::libhal)
