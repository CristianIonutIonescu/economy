cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 14)

project(client)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/")

# Point to Qt cmake files
set(CMAKE_PREFIX_PATH 
    ${CMAKE_PREFIX_PATH} 
    /usr/local/Qt-5.10.0/
    /usr/local/lib/cares/
    )
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(utils_dir "${CMAKE_SOURCE_DIR}/../common")
include_directories(utils_dir)

list(APPEND CMAKE_PREFIX_PATH
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/..
    ${PROJECT_SOURCE_DIR}/../common
)

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

find_package(Qt5 COMPONENTS Core Widgets Charts REQUIRED)

add_executable(client
    main.cpp 
    ${utils_dir}/date.cpp
    ${utils_dir}/date.hpp
    ${utils_dir}/datatypes.grpc.pb.cc
    ${utils_dir}/datatypes.pb.cc
    ${utils_dir}/utils.cpp
    ${utils_dir}/utils.hpp
    chart.cpp
    dataretriever.cpp
    datapresenter.cpp
    )

target_link_libraries(client
    ${PROTOBUF_LIBRARY}
    gRPC::grpc++
    Qt5::Widgets
    Qt5::Charts)
