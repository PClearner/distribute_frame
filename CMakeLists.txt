cmake_minimum_required(VERSION 3.5.0)
project(distribute_frame VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(Protobuf REQUIRED)


include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(/usr/local/include)



link_directories(/usr/local/lib)

set(LOG_FILES
    ${CMAKE_SOURCE_DIR}/src/tools/lock.cpp
    ${CMAKE_SOURCE_DIR}/src/tools/logger.cpp
)

add_executable(log  
    ${LOG_FILES}
    ${CMAKE_SOURCE_DIR}/app/tools/logmain.cpp
)


set(INIT_FILES
    ${LOG_FILES}
    ${CMAKE_SOURCE_DIR}/src/rpc/init.cpp
)

add_executable(inittest 
    ${INIT_FILES}
    ${CMAKE_SOURCE_DIR}/test/inittest.cpp
)

target_link_libraries(inittest
    ${CMAKE_SOURCE_DIR}/lib/libyaml-cpp.a
)

add_executable(protobuftest 
    ${CMAKE_SOURCE_DIR}/test/protobuftest.cpp
    ${CMAKE_SOURCE_DIR}/src/rpc/rpcheader.pb.cc
)

target_link_libraries(protobuftest ${PROTOBUF_LIBRARIES})

set(RPC_FILES
    ${LOG_FILES}
    ${CMAKE_SOURCE_DIR}/src/rpc/init.cpp
    ${CMAKE_SOURCE_DIR}/src/rpc/rpcheader.pb.cc
    ${CMAKE_SOURCE_DIR}/src/rpc/rpcprovider.cpp
    ${CMAKE_SOURCE_DIR}/src/rpc/zookeeperutil.cpp
)

add_executable(provier
    ${CMAKE_SOURCE_DIR}/test/rpc/provider.cpp
    ${RPC_FILES}
)

target_link_libraries(provier 
    ${PROTOBUF_LIBRARIES}
    /usr/lib/x86_64-linux-gnu/libzookeeper_mt.a
    pthread
    ${CMAKE_SOURCE_DIR}/lib/libmuduo_net.a
    ${CMAKE_SOURCE_DIR}/lib/libmuduo_base.a
    ${CMAKE_SOURCE_DIR}/lib/libyaml-cpp.a
    
)

add_subdirectory(${CMAKE_SOURCE_DIR}/app)

