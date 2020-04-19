cmake_minimum_required(VERSION 2.8.1)
set(CMAKE_CXX_FLAGS "-std=c++11")

project(joint)

set(3RD_DIR /home/3rd/)
# Header include
include_directories(
	${3RD_DIR}/include
)

# Source binding
file(GLOB SOURCES 
	src/*.cpp
	)
add_executable(${PROJECT_NAME} ${SOURCES})

# 3rd
# target_link_libraries(${PROJECT_NAME}  ${3RD_DIR}/lib/libglog.a)
target_link_libraries(${PROJECT_NAME}  ${3RD_DIR}/lib/libglog.so)

# system
set(SYS_LIBS pthread)
foreach (LIB ${SYS_LIBS})
    find_library(${LIB} ${LIB})
    if (${LIB})
        target_link_libraries(${PROJECT_NAME} ${LIB})
    else ()
        message(FATAL_ERROR "Can't find ${LIB} library")
    endif ()
endforeach ()
