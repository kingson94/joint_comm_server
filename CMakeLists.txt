cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)

set(PROJ_NAME joint)

#Source binding
set(SOURCES src/main.cpp)
add_executable(${PROJ_NAME} ${SOURCES})
