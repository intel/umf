

set(MD5_DIR ${XMP_3PTY_DIR}/zuid)
set(EXPAT_DIR ${XMP_3PTY_DIR}/expat/lib)

add_definitions(-DHAVE_EXPAT_CONFIG_H)

if(WIN32)
	configure_file(${XMP_MAIN_DIR}/expat_config_win32.h ${CMAKE_BINARY_DIR}/expat_config.h COPYONLY)
elseif(UNIX)
	configure_file(${XMP_MAIN_DIR}/expat_config_linux.h ${CMAKE_BINARY_DIR}/expat_config.h COPYONLY)
endif()
include_directories(${CMAKE_BINARY_DIR})

file(GLOB SOURCE
    "source/*.cpp"
    "source/*.hpp"
)

file(GLOB SOURCE_Common
    ${XMP_COMMON_SOURCE_DIR}/*.cpp
    ${XMP_COMMON_SOURCE_DIR}/*.hpp
)

set(SOURCE_Expat
    ${EXPAT_DIR}/xmlparse.c
    ${EXPAT_DIR}/xmlrole.c
    ${EXPAT_DIR}/xmltok.c
    ${EXPAT_DIR}/expat.h
    ${CMAKE_BINARY_DIR}/expat_config.h
)

file(GLOB SOURCE_MD5
    "source/*.cpp"
    "source/*.hpp"
    ${MD5_DIR}/interfaces/*.cpp
    ${MD5_DIR}/interfaces/*.h
)

if(WIN32)
    list(REMOVE_ITEM SOURCE_Common ${XMP_COMMON_SOURCE_DIR}/Host_IO-UNIX.cpp)
    list(REMOVE_ITEM SOURCE_Common ${XMP_COMMON_SOURCE_DIR}/Host_IO-POSIX.cpp)
    list(REMOVE_ITEM SOURCE_Common ${XMP_COMMON_SOURCE_DIR}/Host_IO-Mac.cpp)
elseif(UNIX)
    list(REMOVE_ITEM SOURCE_Common ${XMP_COMMON_SOURCE_DIR}/Host_IO-Win.cpp)
    list(REMOVE_ITEM SOURCE_Common ${XMP_COMMON_SOURCE_DIR}/Host_IO-UNIX.cpp)
    list(REMOVE_ITEM SOURCE_Common ${XMP_COMMON_SOURCE_DIR}/Host_IO-Mac.cpp)
endif()

add_library(xmpcore OBJECT ${SOURCE} ${SOURCE_MD5} ${SOURCE_Common} ${SOURCE_Expat})
