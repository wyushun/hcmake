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


set(PROTOBUF_LIB protobuf::libprotobuf)
if(CMAKE_CROSSCOMPILING)
  find_program(PROTOC protoc)
else()
  set(PROTOC $<TARGET_FILE:protobuf::protoc>)
  print(TARGET_FILE)
  print(PROTOC)
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

set(REFLECTION_LIB gRPC::grpc++_reflection)
set(GRPCPP_LIB gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
  find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin)
else()
  set(GRPC_CPP_PLUGIN $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
  print(TARGET_FILE)
  print(GRPC_CPP_PLUGIN)
  file(GENERATE OUTPUT "./generator_test.txt" CONTENT "$<TARGET_FILE:gRPC::grpc_cpp_plugin>")
endif()

# Proto file
get_filename_component(hello_proto "./hello.proto" ABSOLUTE)
get_filename_component(my_proto_path "${hello_proto}" PATH)

# Generated sources
set(proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/hello.pb.cc" 
  "${CMAKE_CURRENT_BINARY_DIR}/hello.pb.h" 
  "${CMAKE_CURRENT_BINARY_DIR}/hello.grpc.pb.cc" 
  "${CMAKE_CURRENT_BINARY_DIR}/hello.grpc.pb.h")
add_custom_command(
      OUTPUT ${proto_srcs}
      COMMAND ${PROTOC}
      ARGS --grpc_out ${CMAKE_CURRENT_BINARY_DIR}
        --cpp_out ${CMAKE_CURRENT_BINARY_DIR}
        -I ${my_proto_path}
        --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN}
        ${hello_proto}
      DEPENDS ${hello_proto})

# define proto library
set(GRPC_PROTO_LIB grpc_proto)
add_library(${GRPC_PROTO_LIB} ${proto_srcs})
target_link_libraries(${GRPC_PROTO_LIB}
  ${REFLECTION_LIB}
  ${GRPCPP_LIB}
  ${PROTOBUF_LIB})

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
  target_link_libraries(${ELF} ${GRPC_PROTO_LIB})
endforeach()
