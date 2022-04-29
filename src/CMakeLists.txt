# set target
set(NPU_LIB "npu")
add_library(${NPU_LIB} SHARED)
# add_dependencies(${NPU_LIB} format)

# find all source files
file(GLOB_RECURSE HCMAKE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
message(STATUS "${CMAKE_CURRENT_SOURCE_DIR} SRCS: ${SRCS}")

# assign target include dirs and sources
target_include_directories(${NPU_LIB} PUBLIC ${INC_DIR})
target_sources(${NPU_LIB} PRIVATE ${HCMAKE_SRCS})

# assign target compile options
target_compile_features(${NPU_LIB} PRIVATE cxx_std_11)
target_compile_options(${NPU_LIB} PRIVATE -Wall -Werror -Wextra)

