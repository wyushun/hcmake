
# # fetch and compile tbb
# FetchContent_Declare(
#   tbb
#   GIT_REPOSITORY git@github.com:oneapi-src/oneTBB.git
#   GIT_TAG v2021.5.0
# )
# set(TBB_TEST OFF CACHE BOOL "" FORCE)
# set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
# FetchContent_GetProperties(tbb)
# if(NOT tbb)
#   FetchContent_Populate(tbb)
#   add_subdirectory(${tbb_SOURCE_DIR} ${tbb_BINARY_DIR})
# endif()
# FetchContent_MakeAvailable(tbb)
# message(STATUS "tbb_SOURCE_DIR = ${tbb_SOURCE_DIR}")
# message(STATUS "tbb_BINARY_DIR = ${tbb_BINARY_DIR}")
# add_custom_target(install_tbb ALL
#   COMMAND ${CMAKE_COMMAND} -P cmake_install.cmake
#   WORKING_DIRECTORY ${tbb_BINARY_DIR}
#   DEPENDS tbb)

# # fetch and compile dnn
# FetchContent_Declare(
#   onednn
#   GIT_REPOSITORY git@github.com:oneapi-src/oneDNN.git
#   GIT_TAG v2.5.1
# )
# set(DNNL_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(DNNL_CPU_RUNTIME "TBB" CACHE STRING "")
# set(TBB_ROOT ${CMAKE_INSTALL_PREFIX} CACHE STRING "")
# FetchContent_GetProperties(onednn)
# if(NOT onednn)
#   FetchContent_Populate(onednn)
#   add_subdirectory(${onednn_SOURCE_DIR} ${onednn_BINARY_DIR})
# endif()
# FetchContent_MakeAvailable(onednn)
# message(STATUS "onednn_SOURCE_DIR = ${onednn_SOURCE_DIR}")
# message(STATUS "onednn_BINARY_DIR = ${onednn_BINARY_DIR}")

# add_dependencies(dnnl install_3rd_party)
