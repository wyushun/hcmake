
# -------------------------------------------------------------------
# fetch and compile and install pybind11
# -------------------------------------------------------------------
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY git@github.com:pybind/pybind11.git
  GIT_TAG v2.9.1
  GIT_SHALLOW TRUE
)
FetchContent_GetProperties(pybind11)
if(NOT pybind11_POPULATED)
  FetchContent_Populate(pybind11)
  add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
endif()
FetchContent_MakeAvailable(pybind11)
message(STATUS "pybind11_SOURCE_DIR = ${pybind11_SOURCE_DIR}")
message(STATUS "pybind11_BINARY_DIR = ${pybind11_BINARY_DIR}")
add_custom_target(install_pybind11 ALL
  COMMAND ${CMAKE_COMMAND} -P cmake_install.cmake
  WORKING_DIRECTORY ${pybind11_BINARY_DIR})


# -------------------------------------------------------------------
# build pybind11 exported modules
# -------------------------------------------------------------------
pybind11_add_module(py_math py_math.cpp)
target_compile_features(py_math PUBLIC cxx_std_14)
set_target_properties(py_math PROPERTIES SUFFIX ".so")
