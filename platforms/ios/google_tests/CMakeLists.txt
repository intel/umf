cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
message(STATUS "CMake version: ${CMAKE_VERSION}")

project(gtests)

if (NOT DEFINED VMF_FRAMEWORK)
  message(FATAL_ERROR "Path to vmf.framework is not passed!")
elseif(NOT EXISTS ${VMF_FRAMEWORK})
  message(FATAL_ERROR "vmf.framework is not found!")
endif()

set (IOS_SDK_PATH "/Developer/Platforms/iPhoneOS.platform/Developer/SDKs")
set (TARGETSDK "iPhoneOS8.0.sdk")

set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos")
set (CMAKE_OSX_SYSROOT "${IOS_SDK_PATH}/${TARGETSDK}")

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v -stdlib=libc++")

set(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework vmf")

file(GLOB SRC "../../../modules/vmdatasource/test/*.cpp"
              "../../../modules/vmfcore/test/*.cpp"
              "../../../3rdparty/gtest/fused-src/gtest/gtest-all.cc"
              "../../../3rdparty/gtest/fused-src/gtest/gtest.h"
              "../../../data/BlueSquare.avi")

file(GLOB RSRC_FILES "../../../data/BlueSquare.avi")

list(REMOVE_ITEM SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/vmdatasource/test/unit_test_ds.cpp")
list(REMOVE_ITEM SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/vmfcore/test/test_main.cpp")
list(APPEND SRC "${CMAKE_SOURCE_DIR}/main.mm")

set (MY_HEADERS "${VMF_FRAMEWORK}/vmf.framework" "../../../3rdparty/gtest/fused-src/gtest/")
include_directories(${MY_HEADERS})

message(STATUS 	"HEADERS=${MY_HEADERS}")
message(STATUS 	"SRC=${SRC}")

add_executable(gtests ${SRC} ${RSRC_FILES})

set_target_properties (gtests PROPERTIES MACOSX_BUNDLE TRUE)
set_target_properties (gtests PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
set_target_properties (gtests PROPERTIES RESOURCE ${RSRC_FILES})


set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.intel.\${PRODUCT_NAME:identifier}")
set(APP_TYPE MACOSX_BUNDLE)

find_library(FRAMEWORK_vmf
             NAMES vmf
             PATHS ${VMF_FRAMEWORK}
             PATH_SUFFIXES Frameworks
             NO_DEFAULT_PATH)

MARK_AS_ADVANCED(FRAMEWORK_vmf)
if(NOT FRAMEWORK_vmf)
MESSAGE(ERROR ": Framework vmf not found")
else()
TARGET_LINK_LIBRARIES(gtests "${FRAMEWORK_vmf/vmf}")
MESSAGE(STATUS "Framework vmf found at ${FRAMEWORK_vmf}")
endif()

target_link_libraries(gtests stdc++)


