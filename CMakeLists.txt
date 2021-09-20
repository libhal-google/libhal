cmake_minimum_required(VERSION 3.1...3.21)

project(libembeddedhal VERSION 0.0.1 LANGUAGES CXX)

add_library(libembeddedhal INTERFACE)
target_include_directories(libembeddedhal
                           INTERFACE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>)
target_compile_features(libembeddedhal INTERFACE cxx_std_20)
set_target_properties(libembeddedhal PROPERTIES CXX_EXTENSIONS OFF)

install(TARGETS libembeddedhal DESTINATION "."
        PUBLIC_HEADER DESTINATION include
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)
