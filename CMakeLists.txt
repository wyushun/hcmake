cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
project(hcmake LANGUAGES C CXX)
include(cmake/common.cmake)

print_cache_vars()

# build all targets
add_subdirectory(src)
add_subdirectory(test)

if(BUILD_PYBIND)
  add_subdirectory(py_exports)
endif()

if(BUILD_GRPC)
  add_subdirectory(grpc)
endif()

