cmake_minimum_required(VERSION 2.8.11)
project(libjson)

set(LIBJSON_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/_internal/Source")
file(GLOB SRC ${LIBJSON_SOURCE_DIR}/*.cpp)
list(REMOVE_ITEM SRC 
    ${LIBJSON_SOURCE_DIR}/JSONPreparse.cpp 
    ${LIBJSON_SOURCE_DIR}/JSONNode_Mutex.cpp
    ${LIBJSON_SOURCE_DIR}/JSONMemory.cpp
    ${LIBJSON_SOURCE_DIR}/JSONAllocator.cpp)

file(GLOB_RECURSE HDR src *.h)
#include_directories( ... )

if(WIN32)
    add_definitions(-DUNICODE)
else()
    if(BUILD_SHARED_LIBS)
        add_definitions(-fPIC)
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-private-field")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-shorten-64-to-32")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder")
endif()

add_library(libjson OBJECT ${SRC} ${HDR})
target_compile_definitions(libjson PRIVATE $<$<CONFIG:Debug>:JSON_DEBUG> PRIVATE $<$<CONFIG:Release>:NDEBUG>)
set_target_properties(libjson PROPERTIES FOLDER "3rdparty")
