cmake_minimum_required(VERSION 3.5.0)
project(distribute_frame VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

set(LOG_FILES
    ${CMAKE_SOURCE_DIR}/src/tools/lock.cpp
    ${CMAKE_SOURCE_DIR}/src/tools/logger.cpp
)

add_executable(log  
    ${CMAKE_SOURCE_DIR}/app/tools/logmain.cpp
    ${LOG_FILES}
)



