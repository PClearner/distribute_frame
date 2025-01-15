
set(CMAKE_BUILD_TYPE "Debug")


include_directories(${CMAKE_SOURCE_DIR}/app)

# SET(RPC_lib
#     ${PROTOBUF_LIBRARIES}
#     /usr/lib/x86_64-linux-gnu/libzookeeper_mt.a
#     pthread
#     ${CMAKE_SOURCE_DIR}/lib/libmuduo_net.a
#     ${CMAKE_SOURCE_DIR}/lib/libmuduo_base.a
#     ${CMAKE_SOURCE_DIR}/lib/libyaml-cpp.a
# )


add_executable(provider 
    ${CMAKE_SOURCE_DIR}/app/rpc/provider.cpp
    ${RPC_FILES}
    ${CMAKE_SOURCE_DIR}/app/rpc/userservice.pb.cc
)

target_link_libraries(provider 
    ${PROTOBUF_LIBRARIES}
    /usr/lib/x86_64-linux-gnu/libzookeeper_mt.a
    pthread
    ${CMAKE_SOURCE_DIR}/lib/libmuduo_net.a
    ${CMAKE_SOURCE_DIR}/lib/libmuduo_base.a
    ${CMAKE_SOURCE_DIR}/lib/libyaml-cpp.a
    
)

add_executable(client 
    ${CMAKE_SOURCE_DIR}/app/rpc/client.cpp
    ${RPC_FILES}
    ${CMAKE_SOURCE_DIR}/app/rpc/userservice.pb.cc
)

target_link_libraries(client
    ${PROTOBUF_LIBRARIES}
    /usr/lib/x86_64-linux-gnu/libzookeeper_mt.a
    pthread
    ${CMAKE_SOURCE_DIR}/lib/libmuduo_net.a
    ${CMAKE_SOURCE_DIR}/lib/libmuduo_base.a
    ${CMAKE_SOURCE_DIR}/lib/libyaml-cpp.a
    
)

set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/rpc)
set_target_properties(provider PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/rpc)