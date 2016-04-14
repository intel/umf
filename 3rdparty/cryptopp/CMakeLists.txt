cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)

project(cryptopp)

set(cryptopp_VERSION_MAJOR 5)
set(cryptopp_VERSION_MINOR 6)
set(cryptopp_VERSION_PATCH 3)

include(GNUInstallDirs)
include(TestBigEndian)
include(CheckCXXSymbolExists)

#============================================================================
# Settable options
#============================================================================

option(CRYPTOPP_DISABLE_ASM "Disable ASM" OFF)
option(CRYPTOPP_DISABLE_SSSE3 "Disable SSSE3" OFF)
option(CRYPTOPP_DISABLE_AESNI "Disable AES-NI" OFF)

#============================================================================
# Internal compiler options
#============================================================================

set(LIB_VER ${cryptopp_VERSION_MAJOR}${cryptopp_VERSION_MINOR}${cryptopp_VERSION_PATCH})

if(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
	add_definitions(-wd68 -wd186 -wd279 -wd327 -wd161 -wd3180)
endif()

# Endianess
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
	add_definitions(-DIS_BIG_ENDIAN)
endif()

if(CRYPTOPP_DISABLE_ASM)
	add_definitions(-DCRYPTOPP_DISABLE_ASM)
endif()
if(CRYPTOPP_DISABLE_SSSE3)
	add_definitions(-DCRYPTOPP_DISABLE_SSSE3)
endif()
if(CRYPTOPP_DISABLE_AESNI)
	add_definitions(-DCRYPTOPP_DISABLE_AESNI)
endif()
if(NOT CRYPTOPP_DATA_DIR STREQUAL "")
	add_definitions(-DCRYPTOPP_DATA_DIR=${CRYPTOPP_DATA_DIR})
endif()

#============================================================================
# Sources & headers
#============================================================================

# Library headers
file(GLOB cryptopp_HEADERS *.h)

# Test sources
file(GLOB cryptopp_SOURCES_TEST bench.cpp bench2.cpp test.cpp validat1.cpp validat2.cpp validat3.cpp adhoc.cpp datatest.cpp regtest.cpp fipsalgt.cpp dlltest.cpp fipstest.cpp)

# Library sources
file(GLOB cryptopp_SOURCES *.cpp)
list(REMOVE_ITEM cryptopp_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/cryptlib.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/cpu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/simple.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/winpipes.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/cryptlib_bds.cpp
		${cryptopp_SOURCES_TEST}
		)
set(cryptopp_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/cryptlib.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/cpu.cpp
		${cryptopp_SOURCES}
		)

if(MINGW)
	list(APPEND cryptopp_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/winpipes.cpp)
endif()

if(WIN32 AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
   enable_language(ASM_MASM)
   if(CMAKE_ASM_MASM_COMPILER_WORKS)
         set(cryptopp_SOURCES ${cryptopp_SOURCES} x64dll.asm x64masm.asm)
   else()
         message(WARNING "MASM compiler required for cryptopp win64 compilation wasn't found or doesn't work")
   endif(CMAKE_ASM_MASM_COMPILER_WORKS)
endif(WIN32 AND (CMAKE_SIZEOF_VOID_P EQUAL 8))

#============================================================================
# Compile targets
#============================================================================
add_library(cryptopp STATIC ${cryptopp_SOURCES})

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
       # Enables -fPIC on all 64-bit platforms
       set_target_properties(cryptopp PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
endif()

target_include_directories(cryptopp PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:include/cryptopp>)

if(NOT MSVC)
	set(COMPAT_VERSION ${cryptopp_VERSION_MAJOR}.${cryptopp_VERSION_MINOR})

	set_target_properties(cryptopp
			PROPERTIES
			OUTPUT_NAME cryptopp)
endif()

#============================================================================
# Third-party libraries
#============================================================================
if(WIN32)
	target_link_libraries(cryptopp ws2_32)
endif()

find_package(Threads)
target_link_libraries(cryptopp ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(cryptopp PROPERTIES FOLDER "3rdparty")
