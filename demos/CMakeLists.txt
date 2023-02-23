cmake_minimum_required(VERSION 3.20)

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
arm_cortex_post_build(${PROJECT_NAME})
