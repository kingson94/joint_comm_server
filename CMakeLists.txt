cmake_minimum_required(VERSION 2.8.1)
set(CMAKE_CXX_FLAGS "-std=c++11")

project(joint)

set(3RD_DIR /home/environment/)
# Header include
include_directories(
	${3RD_DIR}/include
    src/
)

# Source binding
file(GLOB SOURCES 
	src/*.cpp
    src/core/*.cpp
    src/core/protocol/*.cpp
    src/utils/*.cpp
	)
add_executable(${PROJECT_NAME} ${SOURCES})

# Libs
set(SYS_LIBS pthread glog)
foreach (LIB ${SYS_LIBS})
    find_library(RESULT ${LIB})
    if (RESULT)
        target_link_libraries(${PROJECT_NAME} ${LIB})
    else ()
        message(FATAL_ERROR "Can't find ${LIB} library")
    endif ()
    unset(RESULT CACHE)
endforeach ()
