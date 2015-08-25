cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
message(STATUS "CMake version: ${CMAKE_VERSION}")

# must go before the `project()` command
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

project(vmf)

set(VMF_3PTY_DIR ${CMAKE_SOURCE_DIR}/3rdparty)
set(XMP_DIR ${VMF_3PTY_DIR}/xmp)
set(MODULES_DIR ${CMAKE_SOURCE_DIR}/modules)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(NOT IOS)
  option(BUILD_SHARED_LIBS "Whether or not to build VMF as shared library" ON)
  option(BUILD_SAMPLES "Whether or not to build the samples" ON)
  option(BUILD_TESTS "Whether or not to build the tests" ON)
else()
  set(BUILD_SHARED_LIBS OFF)
  set(BUILD_SAMPLES OFF)
  set(BUILD_QT_SAMPLES OFF)
  set(BUILD_TESTS OFF)
endif()

if(WIN32)
  set(BUILD_INSTALLER ON)
endif()

if(UNIX)
  option(CODE_COVERAGE "Whether or not to build with code coverage info (disables samples)" OFF)
endif()

if(CMAKE_GENERATOR MATCHES "Makefiles|Ninja" AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(UNIX AND CODE_COVERAGE)
  set(BUILD_SAMPLES OFF)
  set(BUILD_QT_SAMPLES OFF)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# The version number.
set(VMF_VERSION_MAJOR "2")
set(VMF_VERSION_MINOR "0")
set(VMF_VERSION_TYPE  "") # dev, alpha, beta

# reporting build settings
message(STATUS "*** VMF build configuration ***")
message(STATUS "    VMF version: ${VMF_VERSION_MAJOR}.${VMF_VERSION_MINOR}.${VMF_VERSION_TYPE}")
message(STATUS "    CMake generator: ${CMAKE_GENERATOR}")
message(STATUS "    CMake build tool: ${CMAKE_BUILD_TOOL}")
if(NOT CMAKE_GENERATOR MATCHES "Xcode|Visual Studio")
  message(STATUS "    CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
else()
  message(STATUS "    CMAKE_CONFIGURATION_TYPES: ${CMAKE_CONFIGURATION_TYPES}")
endif()
message(STATUS "    BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")
message(STATUS "    BUILD_SAMPLES: ${BUILD_SAMPLES}")
message(STATUS "    BUILD_QT_SAMPLES: ${BUILD_QT_SAMPLES}")
message(STATUS "    BUILD_TESTS: ${BUILD_TESTS}")

#if(WIN32)
    SET(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE INTERNAL "Install path prefix" FORCE)
#endif()

set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

if (UNIX)
    add_definitions(-D_FILE_OFFSET_BITS=64)
endif()

if(WIN32)
    add_definitions(-D_VARIADIC_MAX=10)
else()
    if(ANDROID)
        set(BUILD_SAMPLES OFF)
        message(STATUS "    SW Platform: Linux (Android)")
        message(STATUS "    HW Platform: ${CMAKE_SYSTEM_PROCESSOR}")
        message(STATUS "    NDK Toolchain: ${ANDROID_TOOLCHAIN_NAME} (NDK ${ANDROID_NDK_RELEASE})")
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -z muldefs")
        endif()
    elseif(APPLE)
        if(IOS)
          message(STATUS "    SW Platform: iOS")
        else()
          message(STATUS "    SW Platform: Mac OS")
          if (NOT DEFINED CMAKE_MACOSX_RPATH)
            set(CMAKE_MACOSX_RPATH 0)
          endif()
        endif()
    else()
        message(STATUS "    SW Platform: Linux")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif()

message(STATUS "*** *** ***")
#enable_testing(true)

if(BUILD_TESTS)
    add_subdirectory(${VMF_3PTY_DIR}/gtest)
endif()
add_subdirectory(${VMF_3PTY_DIR}/xmp)
add_subdirectory(${VMF_3PTY_DIR}/libxml2)
add_subdirectory(${VMF_3PTY_DIR}/libjson)
add_subdirectory(modules)

if(BUILD_SAMPLES)
    set(CMAKE_DISABLE_FIND_PACKAGE_VMF TRUE)
    set(VMF_LIB_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(VMF_LIBS vmf)
    set(VMF_INCLUDE_DIR "${MODULES_DIR}/vmfcore/include" "${CMAKE_BINARY_DIR}")
    set(VMF_DATA_DIR "${CMAKE_SOURCE_DIR}/data")
    add_subdirectory(samples)
endif()

if(NOT ANDROID)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        configure_file(Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
        add_custom_target(doc ALL
            ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
    endif()
endif()

if (BUILD_INSTALLER)
    set(CPACK_PACKAGE_NAME "Intel VMF SDK")
    set(CPACK_PACKAGE_VENDOR "Intel")
    set(CPACK_PACKAGE_VERSION "${VMF_VERSION_MAJOR}.${VMF_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_MAJOR ${VMF_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${VMF_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH "0")
    set(CPACK_PACKAGE_FILE_NAME "Intel-VMF-SDK-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
    if(VMF_VERSION_TYPE)
        set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-${VMF_VERSION_TYPE}")
    endif()
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "Intel-VMF-SDK-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
    set(CPACK_GENERATOR "WIX")
    set(CPACK_WIX_PRODUCT_GUID "F05416DA-303D-4952-BEF4-D576AE4F4A3A")
    set(CPACK_WIX_UPGRADE_GUID "4CF37946-64F6-4841-8E8C-6E7BBE2F7654")
    #set(CPACK_WIX_PRODUCT_ICON "")
    #set(CPACK_WIX_UI_BANNER "") 493x58
    #set(CPACK_WIX_UI_DIALOG "") 493x312
    #set(CPACK_WIX_TEMPLATE "") main.wxs

# android libs
    add_custom_target(ANDROID_LIBS
            build_android_libs.py
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Compiling libraries for android" VERBATIM
    )
    if(EXISTS "${CMAKE_SOURCE_DIR}/android_libs")
        install(DIRECTORY "android_libs" DESTINATION ".")
    endif()

# libs built by current project
    # installed in modules/CMakeLists.txt

# For other platforms
    if(EXISTS "${CMAKE_SOURCE_DIR}/to_be_installed")
        file(GLOB TO_BE_INSTALLED "to_be_installed/*")
        foreach(SUBDIR ${TO_BE_INSTALLED})
            install(DIRECTORY ${SUBDIR} DESTINATION ".")
        endforeach()
    endif()

# CMake files
    install(FILES "cmake/FindVMF.cmake" DESTINATION "cmake")

# samples stuff
    install(DIRECTORY "samples" DESTINATION ".")
    install(DIRECTORY "data" DESTINATION ".")
    install(FILES "cmake/FindVMF.cmake" DESTINATION "samples")

# documentation
    install(FILES "docs/VMF_Release_Notes.pdf" DESTINATION "docs")
    install(FILES "docs/VMF_User_guide.pdf" DESTINATION "docs")
    install(FILES "docs/VMF-Specification.pdf" DESTINATION "docs")
    install(FILES "docs/VMF-EULA.docx" DESTINATION "docs")
    install(FILES "LICENSE" DESTINATION ".")
    if(DOXYGEN_FOUND)
        install(DIRECTORY "${CMAKE_BINARY_DIR}/doc/html/" DESTINATION "docs/API-Spec")
    endif()

    include(CPack)
endif()

if(IOS)
    configure_file("${CMAKE_SOURCE_DIR}/platforms/ios/Info.plist.in"
                   "${CMAKE_BINARY_DIR}/ios/Info.plist")
endif()

