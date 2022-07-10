find_package(Threads REQUIRED)

# Find Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
# set(protobuf_MODULE_COMPATIBLE ON CACHE BOOL "")
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
if(Protobuf_FOUND)
  print(Protobuf_FOUND)
  print(Protobuf_VERSION)
  print(Protobuf_INCLUDE_DIR)
  print(Protobuf_INCLUDES)
  print(Protobuf_LIBRARY)
  print(Protobuf_LIBRARIES)
else()
  message(FATAL_ERROR "Protobuf library not found")
endif()


set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
  print(TARGET_FILE)
  print(_PROTOBUF_PROTOC)
endif()

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
if(gRPC_FOUND)
  print(gRPC_FOUND)
  print(gRPC_VERSION)
  print(gRPC_INCLUDE_DIR)
  print(gRPC_INCLUDES)
  print(gRPC_LIBRARY)
  print(gRPC_LIBRARIES)
else()
  message(FATAL_ERROR "gRPC library not found")
endif()

set(_REFLECTION gRPC::grpc++_reflection)
set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
  print(TARGET_FILE)
  print(_GRPC_CPP_PLUGIN_EXECUTABLE)
  file(GENERATE OUTPUT "./generator_test.txt" CONTENT "$<TARGET_FILE:gRPC::grpc_cpp_plugin>")
endif()

# Proto file
get_filename_component(hello_proto "./hello.proto" ABSOLUTE)
get_filename_component(my_proto_path "${hello_proto}" PATH)

# Generated sources
set(proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/hello.pb.cc")
set(proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/hello.pb.h")
set(grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/hello.grpc.pb.cc")
set(grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/hello.grpc.pb.h")
add_custom_command(
      OUTPUT "${proto_srcs}" "${proto_hdrs}" "${grpc_srcs}" "${grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${my_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hello_proto}"
      DEPENDS "${hello_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# define proto library
set(PROTO_LIB grpc_proto)
add_library(${PROTO_LIB}
  ${proto_srcs}
  ${proto_hdrs}
  ${grpc_srcs}
  ${grpc_hdrs})
target_link_libraries(${PROTO_LIB}
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# Targets route_guide_(client|server)
aux_source_directory(. SRCS)
foreach(F ${SRCS})
  get_filename_component(F_PREFIX ${F} NAME_WE)
  set(ELF ${F_PREFIX})
  add_executable(${ELF})
  message(STATUS "Building ELF target: ${ELF}")

  target_sources(${ELF} PRIVATE ${F})
  target_include_directories(${ELF} PRIVATE ${INC_DIR})
  target_compile_features(${ELF} PRIVATE cxx_std_11)
  target_compile_options(${ELF} PRIVATE -Wall)
  target_link_libraries(${ELF} ${PROTO_LIB})
endforeach()
