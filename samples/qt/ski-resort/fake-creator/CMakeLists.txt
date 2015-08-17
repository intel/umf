project(ski-resort-fake-creator)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(VMF)

file(GLOB ski_resort_fake_creator_SOURCES  "*.cpp")

file(GLOB ski_resort_fake_creator_HEADERS  "*.hpp")

if(${WIN32})
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
elseif(${UNIX} AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -Wall -Wno-deprecated")
endif()

include_directories(${VMF_INCLUDE_DIR})
link_directories(${VMF_LIB_DIR})

add_executable(ski_resort_fake_creator
    ${ski_resort_fake_creator_SOURCES}
    ${ski_resort_fake_creator_HEADERS}
)

target_link_libraries(ski_resort_fake_creator ${VMF_LIBS})
set_target_properties(ski_resort_fake_creator PROPERTIES FOLDER "samples")
