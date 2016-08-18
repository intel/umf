# VMF CMake configuration file.
# Implements FIND_PACKAGE(VMF) call in 'Module' mode
#
# Use the following CMake code in your CMakeLists.txt:
# # provide path to the VMF SDK installation dir via 'UMF_DIR' variable (-DUMF_DIR=C:\Intel-VMF-SDK-1.0)
# SET(VMF_STATIC ON) # or 'OFF'
# FIND_PACKAGE(VMF)
# INCLUDE_DIRECTORIES(${UMF_INCLUDE_DIR})
# LINK_DIRECTORIES(${UMF_LIB_DIR})
# TARGET_LINK_LIBRARIES(your_target ${UMF_LIBS})
#
#    The successful 'FIND_PACKAGE(VMF)' call defines the following variables:
#      - UMF_LIBS                  : The list of libraries to links against.
#      - UMF_LIB_DIR               : The directory where VMF lib files are. (Use for 'LINK_DIRECTORIES' call.)
#      - UMF_INCLUDE_DIR           : The VMF include directory.
#      - UMF_BIN_DIR               : The VMF binaries directory.
#      - UMF_DATA_DIR              : The VMF data directory.
#      - UMF_JAR_PATH              : The VMF data directory.


# use static lib unless dynamic is forced via VMF_STATIC or BUILD_SHARED_LIBS
if(NOT DEFINED VMF_STATIC)
  if(BUILD_SHARED_LIBS)
    set(VMF_STATIC OFF)
  else()
    set(VMF_STATIC ON)
  endif()
endif()

if(NOT DEFINED UMF_DIR AND DEFINED ENV{UMF_DIR})
  set(UMF_DIR "$ENV{UMF_DIR}")
endif()

if(NOT DEFINED UMF_DIR)
  if(EXISTS "${CMAKE_SOURCE_DIR}/../include/vmf/vmf.hpp")
    set(UMF_DIR "${CMAKE_SOURCE_DIR}/..")
  else()
    message(FATAL_ERROR "[FindVMF] Can't find VMF. Please specify UMF_DIR to fix.")
  endif()
endif()

set(UMF_LIBS debug vmfd optimized vmf)
set(UMF_DATA_DIR "${UMF_DIR}/data")
set(UMF_INCLUDE_DIR "${UMF_DIR}/include")
set(UMF_LIB_DIR  "${UMF_DIR}")
set(UMF_BIN_DIR  "${UMF_DIR}")

if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
  set(ARCH_DIR "x64")
else()
  set(ARCH_DIR "x86")
endif()
set(UMF_LIB_DIR  "${UMF_LIB_DIR}/${ARCH_DIR}")
set(UMF_BIN_DIR  "${UMF_BIN_DIR}/${ARCH_DIR}")

if(MSVC)
  if(MSVC_VERSION EQUAL 1600)
    set(RUNTIME_SUBDIR vc10)
  elseif(MSVC_VERSION EQUAL 1700)
    set(RUNTIME_SUBDIR vc11)
  elseif(MSVC_VERSION EQUAL 1800)
    set(RUNTIME_SUBDIR vc12)
  else()
    message(FATAL_ERROR "[FindVMF] Unsupported MSVS version. Only MSVS 2010 (vc10), 2012 (vc11) and 2013 (vc12) are supported.")
  endif()
  set(UMF_LIB_DIR  "${UMF_LIB_DIR}/${RUNTIME_SUBDIR}")
  set(UMF_BIN_DIR  "${UMF_BIN_DIR}/${RUNTIME_SUBDIR}")
endif()

if(VMF_STATIC)
  set(UMF_LIB_DIR  "${UMF_LIB_DIR}/static-lib")
else()
  set(UMF_LIB_DIR  "${UMF_LIB_DIR}/lib")
endif()
set(UMF_BIN_DIR  "${UMF_BIN_DIR}/bin")

if(EXISTS "${UMF_INCLUDE_DIR}/vmf/vmf.hpp" AND EXISTS "${UMF_LIB_DIR}")
  set(VMF_FOUND YES)
  message(STATUS "[FindVMF] Using VMF libs from ${UMF_LIB_DIR}")
else()
  set(VMF_FOUND NO)
  message(FATAL_ERROR "[FindVMF] VMF SDK files are absent at '${UMF_INCLUDE_DIR}' or '${UMF_LIB_DIR}'")
endif()

if(EXISTS "${UMF_DIR}/java/umf_java.jar")
  set(UMF_JAR_PATH "${UMF_DIR}/java/umf_java.jar")
endif()

if(VMF_FOUND AND WIN32 AND NOT VMF_STATIC)
  message(STATUS "[FindVMF] Consider adding '${UMF_BIN_DIR}' to PATH before running your application.")
endif()
