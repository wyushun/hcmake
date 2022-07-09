# -------------------------------------------------------------------
# fetch and compile and install googletest and benchmark
# -------------------------------------------------------------------
FetchContent_Declare(
  googletest
  # GIT_REPOSITORY git@github.com:google/googletest.git
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
  GIT_SHALLOW TRUE
)
FetchContent_Declare(
  benchmark
  # GIT_REPOSITORY git@github.com:google/benchmark.git
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.6.0
  GIT_SHALLOW TRUE
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest benchmark)

print(googletest_SOURCE_DIR)
print(googletest_BINARY_DIR)
print(benchmark_SOURCE_DIR)
print(benchmark_BINARY_DIR)

# -------------------------------------------------------------------
# build targets according to test file's name
# -------------------------------------------------------------------
print(CMAKE_CURRENT_SOURCE_DIR)
aux_source_directory(. SRCS)
foreach(F ${SRCS})
  get_filename_component(F_PREFIX ${F} NAME_WE)
  set(ELF ${F_PREFIX})
  add_executable(${ELF})
  message(STATUS "Building ELF target: ${ELF}")

  target_sources(${ELF} PRIVATE ${F})
  target_include_directories(${ELF} PRIVATE ${INC_DIR})
  target_compile_features(${ELF} PRIVATE cxx_std_14)
  target_compile_options(${ELF} PRIVATE -Wall)
  target_link_libraries(${ELF} PUBLIC benchmark gtest hcmake pthread)
endforeach()

