set(PROJ_NAME metadata-write-jpeg-360)
project(${PROJ_NAME})
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(VMF)

include_directories(${UMF_INCLUDE_DIR})
link_directories(${UMF_LIB_DIR})

set(VIDEO_PATH "${UMF_DATA_DIR}/BlueSquare.avi")
set(OUTPUT_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}")

file(GLOB SRC "*.cpp")
add_executable(${PROJ_NAME} ${SRC})
target_link_libraries(${PROJ_NAME} ${UMF_LIBS})
set_target_properties(${PROJ_NAME} PROPERTIES FOLDER "samples")

file(TO_NATIVE_PATH "${VIDEO_PATH}" VIDEO_PATH)
file(TO_NATIVE_PATH "${OUTPUT_PATH}" OUTPUT_PATH)

if(WIN32)
    ADD_CUSTOM_COMMAND(TARGET ${PROJ_NAME}
            POST_BUILD
            COMMAND copy "${VIDEO_PATH}" "${OUTPUT_PATH}"
            )
else()
    ADD_CUSTOM_COMMAND(TARGET ${PROJ_NAME}
            POST_BUILD
            COMMAND cp "${VIDEO_PATH}" "${OUTPUT_PATH}"
            )
endif()
