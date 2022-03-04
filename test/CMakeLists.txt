# -------------------------------------------------------------------
# fetch and compile and install googletest and benchmark
# -------------------------------------------------------------------
FetchContent_Declare(
  googletest
  GIT_REPOSITORY git@github.com:google/googletest.git
  GIT_TAG release-1.11.0
  GIT_SHALLOW TRUE
)
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY git@github.com:google/benchmark.git
  GIT_TAG v1.6.0
  GIT_SHALLOW TRUE
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest benchmark)

message(STATUS "googletest_SOURCE_DIR = ${googletest_SOURCE_DIR}")
message(STATUS "googletest_BINARY_DIR = ${googletest_BINARY_DIR}")
message(STATUS "benchmark_SOURCE_DIR = ${benchmark_SOURCE_DIR}")
message(STATUS "benchmark_BINARY_DIR = ${benchmark_BINARY_DIR}")

add_custom_command(OUTPUT install_googletest
  COMMAND ${CMAKE_COMMAND} -P cmake_install.cmake
  WORKING_DIRECTORY ${googletest_BINARY_DIR}
  DEPENDS gtest gtest_main gmock gmock_main)
add_custom_command(OUTPUT install_benchmark
  COMMAND ${CMAKE_COMMAND} -P cmake_install.cmake
  WORKING_DIRECTORY ${benchmark_BINARY_DIR}
  DEPENDS benchmark benchmark_main)
add_custom_target(install_3rd_party ALL
  DEPENDS install_googletest install_benchmark)

# -------------------------------------------------------------------
# build targets according to test file's name
# -------------------------------------------------------------------
aux_source_directory(. SRCS)
message(STATUS "${CMAKE_CURRENT_SOURCE_DIR} SRCS: ${SRCS}")
foreach(F ${SRCS})
  get_filename_component(F_PREFIX ${F} NAME_WE)
  set(ELF ${F_PREFIX})
  add_executable(${ELF})
  add_dependencies(${ELF} install_3rd_party)
  message(STATUS "Building ELF target: ${ELF}")

  target_sources(${ELF} PRIVATE ${F})
  target_include_directories(${ELF} PRIVATE ${INC_DIR})
  target_compile_features(${ELF} PRIVATE cxx_std_14)
  target_compile_options(${ELF} PRIVATE -Wall -Werror)
  target_link_libraries(${ELF} PUBLIC benchmark gtest hcmake pthread)
endforeach()

