project(ski-resort-visualizer)
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

if(WIN32)
    # static OpenCV is incompatible with Qt5 libs on Windows
    set(tmp_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
    set(BUILD_SHARED_LIBS 1)
endif()

find_package(OpenCV REQUIRED)

if(WIN32)
    set(BUILD_SHARED_LIBS ${tmp_BUILD_SHARED_LIBS})
endif()

find_package(VMF)

file(GLOB ski_resort_visualizer_SOURCES  "*.cpp")

file(GLOB ski_resort_visualizer_HEADERS  "*.hpp")

file(GLOB ski_resort_visualizer_MOC_HEADERS "mainwindow.hpp")

file(GLOB ski_resort_visualizer_FORMS  "*.ui")

file(GLOB ski_resort_visualizer_RESOURCES  "*.qrc")

if(WIN32)
    set(QT_USE_QTMAIN TRUE)
endif()


qt5_wrap_ui(ski_resort_visualizer_FORMS_HEADERS ${ski_resort_visualizer_FORMS})
qt5_add_resources(ski_resort_visualizer_RESOURCES_RCC ${ski_resort_visualizer_RESOURCES})

if(${WIN32})
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
elseif(${UNIX} AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -Wall")
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${VMF_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})
link_directories(${VMF_LIB_DIR})

if(WIN32)
    add_executable(ski_resort_visualizer WIN32
        ${ski_resort_visualizer_SOURCES}
        ${ski_resort_visualizer_HEADERS}
        ${ski_resort_visualizer_MOC_HEADERS}
        ${ski_resort_visualizer_FORMS_HEADERS}
        ${ski_resort_visualizer_RESOURCES_RCC}
    )
else()
    add_executable(ski_resort_visualizer
        ${ski_resort_visualizer_SOURCES}
        ${ski_resort_visualizer_HEADERS}
        ${ski_resort_visualizer_MOC_HEADERS}
        ${ski_resort_visualizer_FORMS_HEADERS}
        ${ski_resort_visualizer_RESOURCES_RCC}
    )
endif()

target_link_libraries(ski_resort_visualizer Qt5::Widgets ${OpenCV_LIBS} ${VMF_LIBS})
set_target_properties(ski_resort_visualizer PROPERTIES FOLDER "samples")
