# -------------------------------------------------------------------
# options and definitions
# -------------------------------------------------------------------
option(USE_LOG "record running log" OFF)
if(USE_LOG)
  add_definitions(-DUSE_LOG)
endif()

# use FetchContent to git clone and compile gtest and benchmark
include(FetchContent)
include(ExternalProject)
set(FETCHCONTENT_QUIET off)

# -------------------------------------------------------------------
# compiler related
# -------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -ggdb -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -ggdb -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")

# -------------------------------------------------------------------
# general vars
# -------------------------------------------------------------------
set(BASE_DIR ${CMAKE_SOURCE_DIR})
set(INC_DIR ${CMAKE_SOURCE_DIR}/inc)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)
# set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/3rd-party/install/)

# for find_xxx functions
# list(APPEND CMAKE_PREFIX_PATH $ENV{CONDA_PREFIX})
# list(APPEND CMAKE_PREFIX_PATH $ENV{HOME}/.local/)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})
message(STATUS "CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_SYSTEM_PREFIX_PATH = ${CMAKE_SYSTEM_PREFIX_PATH}")

# -------------------------------------------------------------------
# use find_library and find_path to find glog
# -------------------------------------------------------------------
find_library(GLOG_LIBRARY
  NAMES glog
  # PATHS ${SELF_DEFINED_PATH}
  REQUIRED
  # NO_DEFAULT_PATH
)
find_path(GLOG_INCLUDE_DIR
  NAMES glog/logging.h
  # PATHS ${SELF_DEFINED_PATH}
  REQUIRED
  # NO_DEFAULT_PATH
)
message(STATUS "GLOG_ROOT = ${GLOG_ROOT}")
message(STATUS "GLOG_LIBRARY = ${GLOG_LIBRARY}")
message(STATUS "GLOG_LIB_PATH = ${GLOG_LIB_PATH}")
message(STATUS "GLOG_INCLUDE_DIR = ${GLOG_INCLUDE_DIR}")
message(STATUS "GLOG_INCLUDE_PATH = ${GLOG_INCLUDE_PATH}")

# -------------------------------------------------------------------
# use find_package to find protobuf
# -------------------------------------------------------------------
#list(APPEND CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX})
# set(protobuf_MODULE_COMPATIBLE ON CACHE BOOL "")
# find_package(Protobuf CONFIG REQUIRED)
#if(Protobuf_FOUND)
#  message(STATUS "Protobuf_FOUND = ${Protobuf_FOUND}")
#  message(STATUS "Protobuf_INCLUDE_DIR = ${Protobuf_INCLUDE_DIR}")
#  message(STATUS "Protobuf_INCLUDES = ${Protobuf_INCLUDES}")
#  message(STATUS "Protobuf_LIBRARY = ${Protobuf_LIBRARY}")
#  message(STATUS "Protobuf_LIBRARIES = ${Protobuf_LIBRARIES}")
#else()
#  message(FATAL_ERROR "Protobuf library not found")
#endif()

# -------------------------------------------------------------------
# define some test or global functions
# -------------------------------------------------------------------
function(print_cache_vars)
  message(STATUS "ARGC: ${ARGC}")
  message(STATUS "ARGV: ${ARGV}")
  message(STATUS "ARGN: ${ARGN}")
  message(STATUS "ARGV0: ${ARGV0}")
  message(STATUS "ARGV1: ${ARGV1}")
  message(STATUS "ARGV2: ${ARGV2}")
  message(STATUS "ARGV3: ${ARGV3}")
  message(STATUS "CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
  message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
  message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
  message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
  message(STATUS "CMAKE_SYSROOT: ${CMAKE_SYSROOT}")
  message(STATUS "CMAKE_ROOT: ${CMAKE_ROOT}")
  message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
  message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
  message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
  message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
  message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
  message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
  message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
  message(STATUS "CMAKE_PROGRAM_PATH: ${CMAKE_PROGRAM_PATH}")
  message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
  set(RET_VAL 100 PARENT_SCOPE)
endfunction()

function(include_dir dir)
  if(IS_DIRECTORY ${dir})
    # include current dir
    message("include dir: " ${dir})
    include_directories(${dir})

    # include sub dir recurrently
    file(GLOB subs RELATIVE ${dir} ${dir}/*)
    foreach(sub ${subs})
      include_dir(${dir}/${sub})
    endforeach()
  endif()
endfunction()

# -------------------------------------------------------------------
# make a target to do clang-format
# -------------------------------------------------------------------
find_program(CLANG_FORMAT clang-format)
if(${CLANG_FORMAT} STREQUAL CLANG_FORMAT-NOTFOUND)
  message(WARNING "clang-format not found!")
else()
  file(GLOB_RECURSE FORMAT_SRCS 
    ${CMAKE_SOURCE_DIR}/inc/*.[ch]pp 
    ${CMAKE_SOURCE_DIR}/src/*.[ch]pp 
    ${CMAKE_SOURCE_DIR}/test/*.[ch]pp)
  message(STATUS "hcmake project's SRCS: ${FORMAT_SRCS}")
  add_custom_command(OUTPUT my_format COMMAND clang-format --verbose -i ${FORMAT_SRCS})
  # add_custom_target(format ALL DEPENDS my_format)
  add_custom_target(format DEPENDS my_format)
endif()
