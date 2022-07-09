# set target
set(HCMAKE_LIB "hcmake")
add_library(${HCMAKE_LIB} SHARED)

# find all source files
file(GLOB_RECURSE HCMAKE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
message(STATUS "${CMAKE_CURRENT_SOURCE_DIR} SRCS: ${SRCS}")

# assign target include dirs and sources
target_include_directories(${HCMAKE_LIB} PUBLIC ${INC_DIR} ${GLOG_INCLUDE_DIR})
target_sources(${HCMAKE_LIB} PRIVATE ${HCMAKE_SRCS})
target_link_libraries(${HCMAKE_LIB} INTERFACE ${GLOG_LIBRARY})

# assign target compile options
target_compile_features(${HCMAKE_LIB} PRIVATE cxx_std_14)
target_compile_definitions(${HCMAKE_LIB} PRIVATE LOG_LEVEL=3)
target_compile_options(${HCMAKE_LIB} PRIVATE -Wall -Werror -Wextra)

