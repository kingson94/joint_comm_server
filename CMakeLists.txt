cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_FLAGS "-std=c++11")

project(joint)

# Header include
include_directories(
    src/
    3rd/include/
)

# Source binding
file(GLOB SOURCES 
	src/*.cpp
    src/util/*.cpp
    src/core/*.cpp
    src/core/base/*.cpp
    src/core/operator/*.cpp
    src/tcp/*.cpp
    src/http/*.cpp
    src/service/*.cpp
    src/service/connection/*.cpp
    src/service/authentication/*.cpp
    src/profile/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# System libs
set(SYS_LIBS pthread ssl crypto)
foreach (LIB ${SYS_LIBS})
    find_library(RESULT ${LIB})
    if (RESULT)
        target_link_libraries(${PROJECT_NAME} ${LIB})
    else ()
        message(FATAL_ERROR "Can't find ${LIB} library")
    endif ()
    unset(RESULT CACHE)
endforeach ()

target_link_libraries(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/3rd/lib/libboost_thread.so")
target_link_libraries(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/3rd/lib/libnghttp2.so")
target_link_libraries(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/3rd/lib/libnghttp2_asio.so")
