set(proj_name metadata-manipulation)
project(${proj_name})
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(VMF)

if(${WIN32})
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
elseif(${UNIX} AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -Wall")
endif()

file(GLOB SRC "*.cpp")
include_directories(${VMF_INCLUDE_DIR})
link_directories(${VMF_LIB_DIR})
add_executable(${proj_name} ${SRC})
target_link_libraries(${proj_name} ${VMF_LIBS})
set_target_properties(${proj_name} PROPERTIES FOLDER "samples")
