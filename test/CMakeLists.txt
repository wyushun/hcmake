# use FetchContent to git clone and compile gtest and benchmark
include(FetchContent)
set(FETCHCONTENT_QUIET off)

# fetch and compile googletest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY git@github.com:google/googletest.git
  GIT_TAG release-1.11.0
)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()
FetchContent_MakeAvailable(googletest)
message(STATUS "googletest_SOURCE_DIR = ${googletest_SOURCE_DIR}")
message(STATUS "googletest_BINARY_DIR = ${googletest_BINARY_DIR}")

# fetch and compile benchmark
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY git@github.com:google/benchmark.git
  GIT_TAG v1.6.0
)
set(BENCHMARK_ENABLE_TESTING OFF)
FetchContent_GetProperties(benchmark)
if(NOT benchmark)
  FetchContent_Populate(benchmark)
  add_subdirectory(${benchmark_SOURCE_DIR} ${benchmark_BINARY_DIR})
endif()
FetchContent_MakeAvailable(benchmark)
message(STATUS "benchmark_SOURCE_DIR = ${benchmark_SOURCE_DIR}")
message(STATUS "benchmark_BINARY_DIR = ${benchmark_BINARY_DIR}")

aux_source_directory(. SRCS)
message(STATUS "${CMAKE_CURRENT_SOURCE_DIR} SRCS: ${SRCS}")

# use for to build targets according to test file's name
foreach(F ${SRCS})
  get_filename_component(F_PREFIX ${F} NAME_WE)
  set(ELF ${F_PREFIX})
  add_executable(${ELF})
  message(STATUS "Building ELF target: ${ELF}")

  target_sources(${ELF} PRIVATE ${F})
  target_link_libraries(${ELF} PUBLIC benchmark gtest hcmake pthread)
  target_include_directories(${ELF} PRIVATE ${INC_DIR})
  target_compile_options(${ELF} PRIVATE -Wall -Werror -Wextra)
endforeach()

