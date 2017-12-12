cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 14)

project(test-parser)

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

add_executable(test-parser 
    main.cpp 
    dataparser.cpp 
    date.cpp
    datatypes.grpc.pb.cc
    datatypes.pb.cc)

MESSAGE(STATUS "GRPC LIBRARY ${GRPC_LIBRARY}")

target_link_libraries(test-parser
    ${PROTOBUF_LIBRARY}
    ${GRPC_LIBRARY})