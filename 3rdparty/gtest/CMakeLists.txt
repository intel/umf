################################################################################
# GTest
# See: http://code.google.com/p/googletest/
################################################################################
cmake_minimum_required(VERSION 2.8)

project(gtest)

if(NOT ANDROID)
    find_package(Threads)
endif()

set(GTEST_HAS_PTHREAD "")

if(ANDROID OR APPLE)
    set(GTEST_HAS_CLONE 0)
else()
    set(GTEST_HAS_CLONE 1)
endif()

set(GTEST_DIR ${VMF_3PTY_DIR}/gtest/fused-src)

set(GTEST_INCLUDE_DIR "${GTEST_DIR}" CACHE PATH "GTest lib include dir" )

include_directories(${GTEST_INCLUDE_DIR})

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-const-variable")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-but-set-variable")
endif()

add_library(gtest STATIC ${GTEST_DIR}/gtest/gtest-all.cc)

if(ANDROID) # Cannot use try_run when crosscompiling
    set(GTEST_HAS_PTHREAD 1)
elseif(CMAKE_USE_PTHREADS_INIT)  # The pthreads library is available and allowed.
    set(GTEST_HAS_PTHREAD 1)
    target_link_libraries(gtest pthread)
else()
    set(GTEST_HAS_PTHREAD 0)
endif()

set_target_properties(gtest PROPERTIES COMPILE_FLAGS "-DGTEST_HAS_PTHREAD=${GTEST_HAS_PTHREAD} -DGTEST_HAS_CLONE=${GTEST_HAS_CLONE}")
set_target_properties(gtest PROPERTIES FOLDER "3rdparty")
