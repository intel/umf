set(PROJ_NAME checksum-utility)
project(${PROJ_NAME})
cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(VMF)

include_directories(${VMF_INCLUDE_DIR})
link_directories(${VMF_LIB_DIR})

file(GLOB SRC "*.cpp")

add_executable(${PROJ_NAME} ${SRC})
target_link_libraries(${PROJ_NAME} ${VMF_LIBS})
set_target_properties(${PROJ_NAME} PROPERTIES FOLDER "samples")

if(${WIN32})
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
elseif(${UNIX} AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -Wall")
endif()
