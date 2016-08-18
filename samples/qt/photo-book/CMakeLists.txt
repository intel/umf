project(photo_book)
cmake_minimum_required(VERSION 2.8.11)

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

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(VMF)
if(NOT ${VMF_FOUND})
    message(FATAL_ERROR "Couldn't found VMF. Stopping build...")
endif()

file(GLOB photo_book_SOURCES
    "*.cpp"
)

file(GLOB photo_book_HEADERS
    "*.hpp"
)

file(GLOB photo_book_QT_HEADERS
    aboutwindow.hpp
    editwindow.hpp
    mainwindow.hpp
    photographics.hpp
    markup_model.hpp
    )

file(GLOB photo_book_FORMS
    "*.ui"
)

file(GLOB photo_book_RESOURCES
    "*.qrc"
)

qt5_wrap_ui(photo_book_FORMS_HEADERS ${photo_book_FORMS})
qt5_add_resources(photo_book_RESOURCES_RCC ${photo_book_RESOURCES})

if(${WIN32})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -Wall")
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${OpenCV_INCLUDE_DIRS} ${UMF_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${UMF_LIB_DIR})

set(CASCADES_PATH "${UMF_DATA_DIR}/haarcascade_frontalface_alt2.xml")
set(CASCADES_OUTPUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}")

file(TO_NATIVE_PATH "${CASCADES_PATH}" CASCADES_PATH)
file(TO_NATIVE_PATH "${CASCADES_OUTPUT}" CASCADES_OUTPUT)

if(WIN32)
    add_executable(photo_book WIN32
        ${photo_book_SOURCES}
        ${photo_book_HEADERS}
        ${photo_book_QT_HEADERS}
        ${photo_book_FORMS_HEADERS}
        ${photo_book_RESOURCES_RCC}
    )

    add_custom_command(TARGET photo_book
            POST_BUILD
            COMMAND copy "${CASCADES_PATH}" "${CASCADES_OUTPUT}"
            )
else()
    add_executable(photo_book
		${photo_book_SOURCES} 
		${photo_book_HEADERS}
		${photo_book_QT_HEADERS}
		${photo_book_FORMS_HEADERS}
		${photo_book_RESOURCES_RCC}
    )

    add_custom_command(TARGET photo_book
        POST_BUILD
        COMMAND cp "${CASCADES_PATH}" "${CASCADES_OUTPUT}"
    )
endif()

target_link_libraries(photo_book Qt5::Widgets ${OpenCV_LIBS} ${UMF_LIBS})
set_target_properties(photo_book PROPERTIES FOLDER "samples")

source_group(QtFiles FILES ${photo_book_QT_HEADERS} ${photo_book_FORMS_HEADERS}
    ${photo_book_RESOURCES_RCC})
source_group(UI FILES ${photo_book_FORMS})
source_group(Resources FILES ${photo_book_RESOURCES})
