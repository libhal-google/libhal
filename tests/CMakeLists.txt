find_package(ut)

add_executable(libembeddedhal.test.exe libembeddedhal/full_scale.test.cpp)

target_link_libraries(libembeddedhal.test.exe
                      PRIVATE
                      libembeddedhal
                      ut)
