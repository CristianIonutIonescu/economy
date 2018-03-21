cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 14)

project(server)

set(utils_dir "${CMAKE_SOURCE_DIR}/../../common")
include_directories(utils_dir)

list(APPEND CMAKE_PREFIX_PATH
    /usr/local/lib/cares/
)

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

add_executable(server
    main.cpp 
    application.cpp
    currencyretriever.cpp
    dataparser.cpp
    ${utils_dir}/date.cpp
    ${utils_dir}/date.hpp
    ${utils_dir}/datatypes.grpc.pb.cc
    ${utils_dir}/datatypes.pb.cc
    ${utils_dir}/utils.cpp
    ${utils_dir}/utils.hpp
    transportservice.cpp
    )

target_link_libraries(server
    ${PROTOBUF_LIBRARY}
    gRPC::grpc++
    libtiny-process-library.so)
