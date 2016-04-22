project(vmf-library)
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(VMF_LIBRARY_NAME "vmf")
set(VMF_JAR_NAME ${VMF_LIBRARY_NAME}_java)
set(VMF_JAR_TEST_NAME ${VMF_LIBRARY_NAME}_java_test)

macro(append_target_property _target _property _value)
    get_target_property(TEMP_VAR ${_target} ${_property})
    if(TEMP_VAR STREQUAL "TEMP_VAR-NOTFOUND")
        set(TEMP_VAR "")
    else()
        set(TEMP_VAR "${TEMP_VAR} ")
    endif()
    set_target_properties(${_target} PROPERTIES ${_property} "${TEMP_VAR}${_value}")
endmacro()

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
    add_definitions(-DUNICODE)
    add_definitions(-DLIBXML_STATIC)
endif()

set(XMP_PUBLIC_DIR "${VMF_3PTY_DIR}/xmp/public/include")
set(LIBXML2_PUBLIC_DIR "${VMF_3PTY_DIR}/libxml2/src/include")
set(LIBJSON_PUBLIC_DIR "${VMF_3PTY_DIR}/libjson/src" "${VMF_3PTY_DIR}/libjson/src/_internal/Source")
set(CRYPTOPP_PUBLIC_DIR "${VMF_3PTY_DIR}/cryptopp")
set(ZLIB_PUBLIC_DIR "${VMF_3PTY_DIR}/xmp/third-party/zlib")
set(XMPCORE_SOURCE_DIR "${VMF_3PTY_DIR}/xmp/XMPCore/source")
set(XMP_ROOT_DIR "${VMF_3PTY_DIR}/xmp")

if (BUILD_JAVA_API)
    find_package(Java 1.7 REQUIRED COMPONENTS Development)
    find_package(JNI REQUIRED)
    include(UseJava)
    include_directories(${JAVA_INCLUDE_PATH} ${JNI_INCLUDE_DIRS})
endif()

if(CODE_COVERAGE)
    add_definitions(--coverage)
endif()

set(XMP_SOURCES $<TARGET_OBJECTS:xmpcore> $<TARGET_OBJECTS:xmpfile>)
set(LIBXML2_SOURCES $<TARGET_OBJECTS:libxml2>)
set(LIBJSON_SOURCES $<TARGET_OBJECTS:libjson>)

set(VMFCORE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vmfcore")
set(VMFCORE_PUBLIC_DIR "${VMFCORE_DIR}/include")
set(VMFCORE_SOURCE_DIR "${VMFCORE_DIR}/src")
set(VMFCORE_DETAILS_DIR "${VMFCORE_SOURCE_DIR}/details")
set(VMFCORE_TESTS_DIR "${VMFCORE_DIR}/test")
set(VMFCORE_TEST_EXECUTABLE "unit-tests")

set(VIDEO_PATH "${CMAKE_SOURCE_DIR}/data/BlueSquare.avi")

if (BUILD_JAVA_API OR ANDROID)
    set(VMFCORE_JAVA_DIR "${VMFCORE_DIR}/java")
    set(VMFCORE_JNI_DIR "${VMFCORE_JAVA_DIR}/jni")
endif()

configure_file("${VMFCORE_PUBLIC_DIR}/vmf/config.hpp.in" "${CMAKE_BINARY_DIR}/config.hpp")

file(GLOB VMFCORE_HEADERS "${VMFCORE_PUBLIC_DIR}/vmf/*.hpp"  "${CMAKE_BINARY_DIR}/config.hpp")
file(GLOB VMFCORE_SOURCES "${VMFCORE_SOURCE_DIR}/*.hpp"  "${VMFCORE_SOURCE_DIR}/*.cpp")
file(GLOB VMFCORE_DETAILS "${VMFCORE_DETAILS_DIR}/*.hpp" "${VMFCORE_DETAILS_DIR}/*.cpp")
file(GLOB VMFCORE_TESTS "${VMFCORE_TESTS_DIR}/*.hpp"     "${VMFCORE_TESTS_DIR}/*.cpp")

if (BUILD_JAVA_API OR ANDROID)
    file(GLOB VMFCORE_JNI "${VMFCORE_JNI_DIR}/*.cpp")
    file(GLOB VMFCORE_JAVA_API "${VMFCORE_JAVA_DIR}/src/com/intel/vmf/*.java")
    file(GLOB VMFCORE_JAVA_TEST "${VMFCORE_JAVA_DIR}/test/*.java")
    file(GLOB VMFCORE_JAVA_JUNIT "${VMFCORE_JAVA_DIR}/test/lib/*.jar")
endif()

source_group(vmfcore\\include FILES ${VMFCORE_HEADERS})
source_group(vmfcore\\src FILES ${VMFCORE_SOURCES})
source_group(vmfcore\\src\\details FILES ${VMFCORE_DETAILS})

if (BUILD_JAVA_API)
    source_group(vmfcore\\jni FILES ${VMFCORE_JNI})
    add_jar (${VMF_JAR_NAME} ${VMFCORE_JAVA_API} OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)
    add_dependencies(${VMF_JAR_NAME} ${VMF_LIBRARY_NAME})
    add_jar (${VMF_JAR_TEST_NAME} ${VMFCORE_JAVA_TEST} INCLUDE_JARS ${VMF_JAR_NAME} ${VMFCORE_JAVA_JUNIT} OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)
    add_dependencies(${VMF_JAR_TEST_NAME} ${VMF_JAR_NAME})
    file(COPY ${VMFCORE_JAVA_JUNIT} DESTINATION ${CMAKE_BINARY_DIR}/bin)
    file(COPY ${VIDEO_PATH} DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()

set(VMDATASOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vmdatasource")
set(VMDATASOURCE_PUBLIC_DIR "${VMDATASOURCE_DIR}/include")
set(VMDATASOURCE_SOURCE_DIR "${VMDATASOURCE_DIR}/src")
set(VMDATASOURCE_TESTS_DIR "${VMDATASOURCE_DIR}/test")
set(VMDATASOURCE_TEST_EXECUTABLE "unit-tests-ds")

file(GLOB VMDATASOURCE_HEADERS "${VMDATASOURCE_PUBLIC_DIR}/vmf/*.hpp")
file(GLOB VMDATASOURCE_SOURCES "${VMDATASOURCE_SOURCE_DIR}/*.hpp" "${VMDATASOURCE_SOURCE_DIR}/*.cpp")
file(GLOB VMDATASOURCE_TESTS "${VMDATASOURCE_TESTS_DIR}/*.hpp" "${VMDATASOURCE_TESTS_DIR}/*.cpp")

source_group(vmdatasource\\src FILES ${VMDATASOURCE_SOURCES})
source_group(vmdatasource\\include FILES ${VMDATASOURCE_HEADERS})

include_directories(${CMAKE_BINARY_DIR} ${VMFCORE_PUBLIC_DIR} ${VMFCORE_DETAILS_DIR} ${VMDATASOURCE_PUBLIC_DIR} ${XMP_PUBLIC_DIR} ${LIBXML2_PUBLIC_DIR} ${LIBJSON_PUBLIC_DIR} ${ZLIB_PUBLIC_DIR} ${CRYPTOPP_PUBLIC_DIR} ${XMPCORE_SOURCE_DIR} ${XMP_ROOT_DIR})

add_library(${VMF_LIBRARY_NAME} ${VMDATASOURCE_HEADERS} ${VMDATASOURCE_SOURCES} ${VMFCORE_HEADERS} ${VMFCORE_SOURCES} ${VMFCORE_DETAILS} ${XMP_SOURCES} ${LIBXML2_SOURCES} ${LIBJSON_SOURCES} ${VMFCORE_JNI})

target_compile_definitions(${VMF_LIBRARY_NAME} PRIVATE $<$<CONFIG:Debug>:JSON_DEBUG> PRIVATE $<$<CONFIG:Release>:NDEBUG>)
if(APPLE AND NOT IOS)
  #set_property(TARGET ${VMF_LIBRARY_NAME} PROPERTY LINK_FLAGS "-framework CoreFoundation -framework CoreServices")
  target_link_libraries(${VMF_LIBRARY_NAME} "-framework CoreFoundation" "-framework CoreServices")
endif()

if(UNIX AND NOT APPLE AND NOT ANDROID)
  # fix build with glibc < 2.17
  target_link_libraries(${VMF_LIBRARY_NAME} rt)
endif()

if(BUILD_SHARED_LIBS AND WIN32)
    append_target_property(${VMF_LIBRARY_NAME} COMPILE_FLAGS "-DVMF_API_EXPORT")
endif()

append_target_property(${VMF_LIBRARY_NAME} COMPILE_FLAGS "-DXMP_StaticBuild=1")

if(WIN32)
    append_target_property(${VMF_LIBRARY_NAME} COMPILE_FLAGS "-DWIN_ENV=1")
elseif(APPLE)
    if(IOS)
       append_target_property(${VMF_LIBRARY_NAME} COMPILE_FLAGS "-DIOS_ENV=1")
       target_link_libraries(${VMF_LIBRARY_NAME} dl)
    else()
       append_target_property(${VMF_LIBRARY_NAME} COMPILE_FLAGS "-DMAC_ENV=1")
       target_link_libraries(${VMF_LIBRARY_NAME} dl)
    endif()
elseif(UNIX)
    append_target_property(${VMF_LIBRARY_NAME} COMPILE_FLAGS "-DUNIX_ENV=1")
    target_link_libraries(${VMF_LIBRARY_NAME} dl)
endif()

target_link_libraries(${VMF_LIBRARY_NAME} cryptopp)

set_target_properties(${VMF_LIBRARY_NAME} PROPERTIES FOLDER "vmf library")

include_directories(${GTEST_INCLUDE_DIR})

if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
  set(ARCH_DIR "x64")
else()
  set(ARCH_DIR "x86")
endif()

if(ANDROID)
  set(VMF_LIB_INSTALL_DIR "android-lib")
else()
  set(VMF_LIB_INSTALL_DIR  ${ARCH_DIR})
  set(VMF_BIN_INSTALL_DIR  ${ARCH_DIR})
endif()

if(MSVC)
  if(MSVC_VERSION EQUAL 1600)
    set(RUNTIME_SUBDIR vc10)
  elseif(MSVC_VERSION EQUAL 1700)
    set(RUNTIME_SUBDIR vc11)
  elseif(MSVC_VERSION EQUAL 1800)
    set(RUNTIME_SUBDIR vc12)
  elseif(MSVC_VERSION EQUAL 1900)
    set(RUNTIME_SUBDIR vc14)
  endif()
  set(VMF_LIB_INSTALL_DIR  "${VMF_LIB_INSTALL_DIR}/${RUNTIME_SUBDIR}")
  set(VMF_BIN_INSTALL_DIR  "${VMF_BIN_INSTALL_DIR}/${RUNTIME_SUBDIR}")
endif()

if(BUILD_SHARED_LIBS)
  if(ANDROID)
    set(VMF_LIB_INSTALL_DIR  "${VMF_LIB_INSTALL_DIR}/libs")
  else()
    set(VMF_LIB_INSTALL_DIR  "${VMF_LIB_INSTALL_DIR}/lib")
  endif()
else()
  set(VMF_LIB_INSTALL_DIR  "${VMF_LIB_INSTALL_DIR}/static-lib")
endif()

if(ANDROID)
  set(VMF_LIB_INSTALL_DIR "${VMF_LIB_INSTALL_DIR}/${ANDROID_NDK_ABI_NAME}")
endif()

set(VMF_BIN_INSTALL_DIR  "${VMF_BIN_INSTALL_DIR}/bin")

install(TARGETS ${VMF_LIBRARY_NAME}
        RUNTIME DESTINATION ${VMF_BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${VMF_LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${VMF_LIB_INSTALL_DIR}
)

# .pdb
if(MSVC AND NOT BUILD_SHARED_LIBS AND CMAKE_VERSION VERSION_LESS 2.8.12)
  # CMake 2.8.12+ brokes PDBs for STATIC libs
  get_target_property(fname ${VMF_LIBRARY_NAME} LOCATION_DEBUG)
  if(fname MATCHES "\\.lib$")
    string(REGEX REPLACE "\\.lib$" ".pdb" fname "${fname}")
    install(FILES ${fname} DESTINATION ${VMF_LIB_INSTALL_DIR} CONFIGURATIONS Debug)
  endif()
endif()

if(ANDROID)
  install(FILES ${VMFCORE_HEADERS} DESTINATION "android-lib/jni/include/vmf")
  install(FILES ${VMDATASOURCE_HEADERS} DESTINATION "android-lib/jni/include/vmf")
else()
  install(FILES ${VMFCORE_HEADERS} DESTINATION "include/vmf")
  install(FILES ${VMDATASOURCE_HEADERS} DESTINATION "include/vmf")
endif()

if(ANDROID)
  install(DIRECTORY "vmfcore/android-lib" DESTINATION ".")
  install(DIRECTORY "vmfcore/java/src" DESTINATION "android-lib")
endif()

if(CODE_COVERAGE)
    append_target_property(${VMF_LIBRARY_NAME} LINK_FLAGS "--coverage")
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
    append_target_property(${VMF_LIBRARY_NAME} LINK_FLAGS "-Wl,-z,muldefs")
endif()

# tests
if(BUILD_TESTS)
    add_executable(${VMDATASOURCE_TEST_EXECUTABLE} ${VMDATASOURCE_TESTS})
    target_link_libraries(${VMDATASOURCE_TEST_EXECUTABLE} gtest vmf)
    set_target_properties(${VMDATASOURCE_TEST_EXECUTABLE} PROPERTIES FOLDER "tests")

    add_executable(${VMFCORE_TEST_EXECUTABLE} ${VMFCORE_TESTS})
    target_link_libraries(${VMFCORE_TEST_EXECUTABLE} gtest vmf)
    set_target_properties(${VMFCORE_TEST_EXECUTABLE} PROPERTIES FOLDER "tests")

    if(CODE_COVERAGE)
        append_target_property(${VMFCORE_TEST_EXECUTABLE} LINK_FLAGS "--coverage")
        append_target_property(${VMDATASOURCE_TEST_EXECUTABLE} LINK_FLAGS "--coverage")
    endif()

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
        append_target_property(${VMFCORE_TEST_EXECUTABLE} LINK_FLAGS "-Wl,-z,muldefs")
        append_target_property(${VMDATASOURCE_TEST_EXECUTABLE} LINK_FLAGS "-Wl,-z,muldefs")
    endif()

    set(OUTPUT_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}")

    if (CMAKE_HOST_WIN32)
        macro(make_windows_path pathname)
          # An extra \\ escape is necessary to get a \ through CMake's processing.
          STRING(REPLACE "/" "\\" ${pathname} "${${pathname}}")
          # Enclose with UNESCAPED quotes.  This means we need to escape our
          # quotes once here, i.e. with \"
          SET(${pathname} \"${${pathname}}\")
        endmacro(make_windows_path)
        make_windows_path(VIDEO_PATH)
        make_windows_path(OUTPUT_PATH)
        add_custom_command(TARGET ${VMDATASOURCE_TEST_EXECUTABLE}
            POST_BUILD
            COMMAND copy "${VIDEO_PATH}" "${OUTPUT_PATH}")
    else()
        add_custom_command(TARGET ${VMDATASOURCE_TEST_EXECUTABLE}
            POST_BUILD
            COMMAND cp "${VIDEO_PATH}" "${OUTPUT_PATH}")
    endif()
endif(BUILD_TESTS)
