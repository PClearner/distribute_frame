
set(CMAKE_BUILD_TYPE "Debug")


SET(mapreduce_lib
    /usr/lib/x86_64-linux-gnu/libzookeeper_mt.a
    pthread
    ${CMAKE_SOURCE_DIR}/lib/libmuduo_net.a
    ${CMAKE_SOURCE_DIR}/lib/libmuduo_base.a
    ${CMAKE_SOURCE_DIR}/lib/libyaml-cpp.a
)


set(MAP_FILES
    ${LOG_FILES}
    ${CMAKE_SOURCE_DIR}/src/rpc/init.cpp
    ${CMAKE_SOURCE_DIR}/src/rpc/zookeeperutil.cpp
    ${CMAKE_SOURCE_DIR}/src/mapreduce/worker.cpp
)


add_executable(map1 
    ${MAP_FILES}
    ${CMAKE_SOURCE_DIR}/app/mapreduce/map1_test.cpp
    
)

target_link_libraries(map1 
    ${mapreduce_lib}
)


set_target_properties(map1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/mapreduce)

add_executable(map2 
    ${MAP_FILES}
    ${CMAKE_SOURCE_DIR}/app/mapreduce/map2_test.cpp
    
)

target_link_libraries(map2 
    ${mapreduce_lib}
)


set_target_properties(map2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/mapreduce)

set(REDUCE_FILES
    ${LOG_FILES}
    ${CMAKE_SOURCE_DIR}/src/rpc/init.cpp
    ${CMAKE_SOURCE_DIR}/src/rpc/zookeeperutil.cpp
    ${CMAKE_SOURCE_DIR}/src/mapreduce/worker.cpp
)

add_executable(reduce1
    ${REDUCE_FILES}
    ${CMAKE_SOURCE_DIR}/app/mapreduce/reduce1_test.cpp
    
)

target_link_libraries(reduce1 
    ${mapreduce_lib}
)


set_target_properties(reduce1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/mapreduce)



add_executable(reduce2
    ${REDUCE_FILES}
    ${CMAKE_SOURCE_DIR}/app/mapreduce/reduce2_test.cpp
    
)

target_link_libraries(reduce2 
    ${mapreduce_lib}
)


set_target_properties(reduce2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/mapreduce)


set(MASTER_FILES
    ${LOG_FILES}
    ${CMAKE_SOURCE_DIR}/src/rpc/init.cpp
    ${CMAKE_SOURCE_DIR}/src/rpc/zookeeperutil.cpp
    ${CMAKE_SOURCE_DIR}/src/mapreduce/master.cpp
)

add_executable(master
    ${MASTER_FILES}
    ${CMAKE_SOURCE_DIR}/app/mapreduce/master_test.cpp
    
)

target_link_libraries(master 
    ${mapreduce_lib}
)


set_target_properties(master PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/mapreduce)
set_target_properties(master PROPERTIES COMPILE_FLAGS "-g -O0")
