set(proj_name metadata-schema)
project(${proj_name})
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(VMF)

set(VIDEO_PATH "${VMF_DATA_DIR}/BlueSquare.avi")
set(SCHEMA_PATH "${VMF_DATA_DIR}/demo_schema.xml")
set(OUTPUT_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}")

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

file(TO_NATIVE_PATH "${VIDEO_PATH}" VIDEO_PATH)
file(TO_NATIVE_PATH "${SCHEMA_PATH}" SCHEMA_PATH)
file(TO_NATIVE_PATH "${OUTPUT_PATH}" OUTPUT_PATH)

if(WIN32)
    ADD_CUSTOM_COMMAND(TARGET ${proj_name}
	    POST_BUILD
	    COMMAND copy "${VIDEO_PATH}" "${OUTPUT_PATH}"
	    COMMAND copy "${SCHEMA_PATH}" "${OUTPUT_PATH}"
	    )
else()
    ADD_CUSTOM_COMMAND(TARGET ${proj_name}
	    POST_BUILD
	    COMMAND cp "${VIDEO_PATH}" "${OUTPUT_PATH}"
            COMMAND cp "${SCHEMA_PATH}" "${OUTPUT_PATH}"
	    )
endif()