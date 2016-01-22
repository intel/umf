cmake_minimum_required(VERSION 2.8)

project(xmp)

#this adds a preprocessor definition to the project
if(WIN32)
    add_definitions(-DWIN_ENV=1)
    add_definitions(-DXMP_WinBuild=1)
    add_definitions(-DUNICODE)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /wd4244 /wd4267")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4267")
else()
    if(BUILD_SHARED_LIBS)
        add_definitions(-fPIC)
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-const-variable")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-const-variable")
    elseif(CMAKE_COMPILER_IS_GNUCXX)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-but-set-variable")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-but-set-variable")
    endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-function-declaration")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-multichar")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-int-to-pointer-cast")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-strict-aliasing")

    if(APPLE)
        if(IOS)
          add_definitions(-DXMP_iOSBuild=1)
          add_definitions(-DIOS_ENV=1) 
        else()
          add_definitions(-DXMP_MacBuild=1)
          add_definitions(-DMAC_ENV=1)
        endif()
    else()
        add_definitions(-DXMP_UnixBuild=1)
        add_definitions(-DUNIX_ENV=1)
        if(ANDROID)
            add_definitions(-DkBigEndianHost=0)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-format")
        endif()
    endif()
endif()

add_definitions(-DXMP_StaticBuild=1)
add_definitions(-DXML_STATIC=1)

set(XMP_3PTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party)
set(XMP_COMMON_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(XMP_MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(LIBRARY_OUTPUT_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/public/include
    ${XMP_3PTY_DIR}/expat/lib
    ${XMP_3PTY_DIR}/zlib
    ${XMP_3PTY_DIR}/zuid
    ${CMAKE_CURRENT_SOURCE_DIR}/XMPFilesPlugins/api/source
    )

add_subdirectory(XMPCore)
add_subdirectory(XMPFiles)

set_target_properties(xmpcore PROPERTIES FOLDER "3rdparty")
set_target_properties(xmpfile PROPERTIES FOLDER "3rdparty")
