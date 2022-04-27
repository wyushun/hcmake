# -------------------------------------------------------------------
# fetch and compile and install pybind11
# -------------------------------------------------------------------
set(PYBIND11_INSTALL ON)
set(PYBIND11_TEST ON)
FetchContent_Declare(
  pybind11
  # GIT_REPOSITORY git@github.com:pybind/pybind11.git
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG v2.9.1
  GIT_SHALLOW TRUE
)
if(NOT pybind11_POPULATED)
  FetchContent_Populate(pybind11)
  add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
endif()
message(STATUS "pybind11_SOURCE_DIR = ${pybind11_SOURCE_DIR}")
message(STATUS "pybind11_BINARY_DIR = ${pybind11_BINARY_DIR}")
add_custom_target(install_pybind11 ALL
  COMMAND ${CMAKE_COMMAND} -P cmake_install.cmake
  WORKING_DIRECTORY ${pybind11_BINARY_DIR})
  # DEPENDS cross_module_gil_utils pybind11_cross_module_tests pybind11_tests)

# -------------------------------------------------------------------
# build pybind11 exported modules
# -------------------------------------------------------------------
pybind11_add_module(py_math py_math.cpp)
target_compile_features(py_math PUBLIC cxx_std_14)
set_target_properties(py_math PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}" SUFFIX "${PYTHON_MODULE_EXTENSION}")

