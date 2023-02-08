cmake_minimum_required(VERSION 3.15)

function(remove_gcda)
  file(GLOB_RECURSE GCDA_FILES ${CMAKE_BINARY_DIR}/**/*.gcda)

  if(GCDA_FILES)
    file(REMOVE ${GCDA_FILES})
  endif()
endfunction()

remove_gcda()