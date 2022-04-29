# -------------------------------------------------------------------
# build targets according to test file's name
# -------------------------------------------------------------------
aux_source_directory(. SRCS)
message(STATUS "${CMAKE_CURRENT_SOURCE_DIR} SRCS: ${SRCS}")
foreach(F ${SRCS})
  get_filename_component(F_PREFIX ${F} NAME_WE)
  set(ELF ${F_PREFIX})
  add_executable(${ELF})
  message(STATUS "Building ELF target: ${ELF}")

  target_sources(${ELF} PRIVATE ${F})
  target_include_directories(${ELF} PRIVATE ${INC_DIR})
  target_compile_features(${ELF} PRIVATE cxx_std_11)
  target_compile_options(${ELF} PRIVATE -Wall -Werror)
  target_link_libraries(${ELF} PUBLIC npuv1 pthread)
endforeach()

