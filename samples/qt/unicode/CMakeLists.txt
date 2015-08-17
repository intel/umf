project(sample-unicode)
cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Qt5
if(NOT DEFINED QT5_DIR AND DEFINED ENV{QT5_DIR})
    set(QT5_DIR $ENV{QT5_DIR})
endif()
if(QT5_DIR)
    set(CMAKE_PREFIX_PATH ${QT5_DIR})
endif()
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets REQUIRED)

find_package(VMF)

if(WIN32)
    set(QT_USE_QTMAIN TRUE)
endif()

if(${WIN32})
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
elseif(${UNIX} AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -Wall")
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${VMF_INCLUDE_DIR})
link_directories(${VMF_LIB_DIR})

set(VIDEO_PATH "${VMF_DATA_DIR}/BlueSquare.avi")
set(OUTPUT_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}")

if(WIN32)
    add_executable(sample-unicode WIN32 main.cpp)
else()
    add_executable(sample-unicode main.cpp)
endif()

target_link_libraries(sample-unicode Qt5::Widgets ${VMF_LIBS})
set_target_properties(sample-unicode PROPERTIES FOLDER "samples")

file(TO_NATIVE_PATH "${VIDEO_PATH}" VIDEO_PATH)
file(TO_NATIVE_PATH "${OUTPUT_PATH}" OUTPUT_PATH)

if (WIN32)
    add_custom_command(TARGET sample-unicode
        POST_BUILD
        COMMAND copy "${VIDEO_PATH}" "${OUTPUT_PATH}"
        )
else()
    add_custom_command(TARGET sample-unicode
        POST_BUILD
        COMMAND cp "${VIDEO_PATH}" "${OUTPUT_PATH}"
        )
endif()
