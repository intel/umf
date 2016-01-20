set(proj_name compression-samples)
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

include_directories(${VMF_INCLUDE_DIR})
link_directories(${VMF_LIB_DIR})

add_executable(compression-builtin compression_builtin.cpp compress_common.cpp)
target_link_libraries(compression-builtin ${VMF_LIBS})
set_target_properties(compression-builtin PROPERTIES FOLDER "samples")

add_executable(compression-custom compression_custom.cpp compress_common.cpp my_lzw.cpp)
target_link_libraries(compression-custom ${VMF_LIBS})
set_target_properties(compression-custom PROPERTIES FOLDER "samples")
