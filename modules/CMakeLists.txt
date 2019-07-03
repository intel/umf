project(umf-library)
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(UMF_LIBRARY_NAME "umf")
set(UMF_JAR_NAME ${UMF_LIBRARY_NAME}_java)
set(UMF_JAR_TEST_NAME ${UMF_LIBRARY_NAME}_java_test)

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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
    add_definitions(-DUNICODE)
    add_definitions(-DLIBXML_STATIC)
endif()

set(XMP_PUBLIC_DIR "${UMF_3PTY_DIR}/xmp/public/include")
set(LIBXML2_PUBLIC_DIR "${UMF_3PTY_DIR}/libxml2/src/include")
set(LIBJSON_PUBLIC_DIR "${UMF_3PTY_DIR}/libjson/src" "${UMF_3PTY_DIR}/libjson/src/_internal/Source")
set(CRYPTOPP_PUBLIC_DIR "${UMF_3PTY_DIR}/cryptopp")
set(ZLIB_PUBLIC_DIR "${UMF_3PTY_DIR}/xmp/third-party/zlib")
set(XMPCORE_SOURCE_DIR "${UMF_3PTY_DIR}/xmp/XMPCore/source")
set(XMP_ROOT_DIR "${UMF_3PTY_DIR}/xmp")

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

set(UMFCORE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/umfcore")
set(UMFCORE_PUBLIC_DIR "${UMFCORE_DIR}/include")
set(UMFCORE_SOURCE_DIR "${UMFCORE_DIR}/src")
set(UMFCORE_DETAILS_DIR "${UMFCORE_SOURCE_DIR}/details")
set(UMFCORE_TESTS_DIR "${UMFCORE_DIR}/test")
set(UMFCORE_TEST_EXECUTABLE "unit-tests")

set(VIDEO_PATH "${CMAKE_SOURCE_DIR}/data/BlueSquare.avi")

if (BUILD_JAVA_API OR ANDROID)
    set(UMFCORE_JAVA_DIR "${UMFCORE_DIR}/java")
    set(UMFCORE_JNI_DIR "${UMFCORE_JAVA_DIR}/jni")
endif()

configure_file("${UMFCORE_PUBLIC_DIR}/umf/config.hpp.in" "${CMAKE_BINARY_DIR}/config.hpp")

file(GLOB UMFCORE_HEADERS "${UMFCORE_PUBLIC_DIR}/umf/*.hpp"  "${CMAKE_BINARY_DIR}/config.hpp")
file(GLOB UMFCORE_SOURCES "${UMFCORE_SOURCE_DIR}/*.hpp"  "${UMFCORE_SOURCE_DIR}/*.cpp")
file(GLOB UMFCORE_DETAILS "${UMFCORE_DETAILS_DIR}/*.hpp" "${UMFCORE_DETAILS_DIR}/*.cpp")
file(GLOB UMFCORE_TESTS "${UMFCORE_TESTS_DIR}/*.hpp"     "${UMFCORE_TESTS_DIR}/*.cpp")

if (BUILD_JAVA_API OR ANDROID)
    file(GLOB UMFCORE_JNI "${UMFCORE_JNI_DIR}/*.cpp")
    file(GLOB UMFCORE_JAVA_API "${UMFCORE_JAVA_DIR}/src/com/intel/umf/*.java")
    file(GLOB UMFCORE_JAVA_TEST "${UMFCORE_JAVA_DIR}/test/*.java")
    file(GLOB UMFCORE_JAVA_JUNIT "${UMFCORE_JAVA_DIR}/test/lib/*.jar")
endif()

source_group(umfcore\\include FILES ${UMFCORE_HEADERS})
source_group(umfcore\\src FILES ${UMFCORE_SOURCES})
source_group(umfcore\\src\\details FILES ${UMFCORE_DETAILS})

if (BUILD_JAVA_API)
    source_group(umfcore\\jni FILES ${UMFCORE_JNI})
    add_jar (${UMF_JAR_NAME} ${UMFCORE_JAVA_API} OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)
    add_dependencies(${UMF_JAR_NAME} ${UMF_LIBRARY_NAME})
    add_jar (${UMF_JAR_TEST_NAME} ${UMFCORE_JAVA_TEST} INCLUDE_JARS ${UMF_JAR_NAME} ${UMFCORE_JAVA_JUNIT} OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)
    add_dependencies(${UMF_JAR_TEST_NAME} ${UMF_JAR_NAME})
    file(COPY ${UMFCORE_JAVA_JUNIT} DESTINATION ${CMAKE_BINARY_DIR}/bin)
    file(COPY ${VIDEO_PATH} DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()

set(VMDATASOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/umdatasource")
set(VMDATASOURCE_PUBLIC_DIR "${VMDATASOURCE_DIR}/include")
set(VMDATASOURCE_SOURCE_DIR "${VMDATASOURCE_DIR}/src")
set(VMDATASOURCE_TESTS_DIR "${VMDATASOURCE_DIR}/test")
set(VMDATASOURCE_TEST_EXECUTABLE "unit-tests-ds")

file(GLOB VMDATASOURCE_HEADERS "${VMDATASOURCE_PUBLIC_DIR}/umf/*.hpp")
file(GLOB VMDATASOURCE_SOURCES "${VMDATASOURCE_SOURCE_DIR}/*.hpp" "${VMDATASOURCE_SOURCE_DIR}/*.cpp")
file(GLOB VMDATASOURCE_TESTS "${VMDATASOURCE_TESTS_DIR}/*.hpp" "${VMDATASOURCE_TESTS_DIR}/*.cpp")

source_group(umdatasource\\src FILES ${VMDATASOURCE_SOURCES})
source_group(umdatasource\\include FILES ${VMDATASOURCE_HEADERS})

include_directories(${CMAKE_BINARY_DIR} ${UMFCORE_PUBLIC_DIR} ${UMFCORE_DETAILS_DIR} ${VMDATASOURCE_PUBLIC_DIR} ${XMP_PUBLIC_DIR} ${LIBXML2_PUBLIC_DIR} ${LIBJSON_PUBLIC_DIR} ${ZLIB_PUBLIC_DIR} ${CRYPTOPP_PUBLIC_DIR} ${XMPCORE_SOURCE_DIR} ${XMP_ROOT_DIR})

add_library(${UMF_LIBRARY_NAME} ${VMDATASOURCE_HEADERS} ${VMDATASOURCE_SOURCES} ${UMFCORE_HEADERS} ${UMFCORE_SOURCES} ${UMFCORE_DETAILS} ${XMP_SOURCES} ${LIBXML2_SOURCES} ${LIBJSON_SOURCES} ${UMFCORE_JNI})

target_compile_definitions(${UMF_LIBRARY_NAME} PRIVATE $<$<CONFIG:Debug>:JSON_DEBUG> PRIVATE $<$<CONFIG:Release>:NDEBUG>)
if(APPLE AND NOT IOS)
  #set_property(TARGET ${UMF_LIBRARY_NAME} PROPERTY LINK_FLAGS "-framework CoreFoundation -framework CoreServices")
  target_link_libraries(${UMF_LIBRARY_NAME} "-framework CoreFoundation" "-framework CoreServices")
endif()

if(UNIX AND NOT APPLE AND NOT ANDROID)
  # fix build with glibc < 2.17
  target_link_libraries(${UMF_LIBRARY_NAME} rt)
endif()

if(BUILD_SHARED_LIBS AND WIN32)
    append_target_property(${UMF_LIBRARY_NAME} COMPILE_FLAGS "-DUMF_API_EXPORT")
endif()

append_target_property(${UMF_LIBRARY_NAME} COMPILE_FLAGS "-DXMP_StaticBuild=1")

if(WIN32)
    append_target_property(${UMF_LIBRARY_NAME} COMPILE_FLAGS "-DWIN_ENV=1")
elseif(APPLE)
    if(IOS)
       append_target_property(${UMF_LIBRARY_NAME} COMPILE_FLAGS "-DIOS_ENV=1")
       target_link_libraries(${UMF_LIBRARY_NAME} dl)
    else()
       append_target_property(${UMF_LIBRARY_NAME} COMPILE_FLAGS "-DMAC_ENV=1")
       target_link_libraries(${UMF_LIBRARY_NAME} dl)
    endif()
elseif(UNIX)
    append_target_property(${UMF_LIBRARY_NAME} COMPILE_FLAGS "-DUNIX_ENV=1")
    target_link_libraries(${UMF_LIBRARY_NAME} dl)
endif()

target_link_libraries(${UMF_LIBRARY_NAME} cryptopp)

set_target_properties(${UMF_LIBRARY_NAME} PROPERTIES FOLDER "umf library")

include_directories(${GTEST_INCLUDE_DIR})

if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
  set(ARCH_DIR "x64")
else()
  set(ARCH_DIR "x86")
endif()

if(ANDROID)
  set(UMF_LIB_INSTALL_DIR "android-lib")
else()
  set(UMF_LIB_INSTALL_DIR  ${ARCH_DIR})
  set(UMF_BIN_INSTALL_DIR  ${ARCH_DIR})
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
  set(UMF_LIB_INSTALL_DIR  "${UMF_LIB_INSTALL_DIR}/${RUNTIME_SUBDIR}")
  set(UMF_BIN_INSTALL_DIR  "${UMF_BIN_INSTALL_DIR}/${RUNTIME_SUBDIR}")
endif()

if(BUILD_SHARED_LIBS)
  if(ANDROID)
    set(UMF_LIB_INSTALL_DIR  "${UMF_LIB_INSTALL_DIR}/libs")
  else()
    set(UMF_LIB_INSTALL_DIR  "${UMF_LIB_INSTALL_DIR}/lib")
  endif()
else()
  set(UMF_LIB_INSTALL_DIR  "${UMF_LIB_INSTALL_DIR}/static-lib")
endif()

if(ANDROID)
  set(UMF_LIB_INSTALL_DIR "${UMF_LIB_INSTALL_DIR}/${ANDROID_NDK_ABI_NAME}")
endif()

set(UMF_BIN_INSTALL_DIR  "${UMF_BIN_INSTALL_DIR}/bin")

install(TARGETS ${UMF_LIBRARY_NAME}
        RUNTIME DESTINATION ${UMF_BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${UMF_LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${UMF_LIB_INSTALL_DIR}
)

# .pdb
if(MSVC AND NOT BUILD_SHARED_LIBS AND CMAKE_VERSION VERSION_LESS 2.8.12)
  # CMake 2.8.12+ brokes PDBs for STATIC libs
  get_target_property(fname ${UMF_LIBRARY_NAME} LOCATION_DEBUG)
  if(fname MATCHES "\\.lib$")
    string(REGEX REPLACE "\\.lib$" ".pdb" fname "${fname}")
    install(FILES ${fname} DESTINATION ${UMF_LIB_INSTALL_DIR} CONFIGURATIONS Debug)
  endif()
endif()

if(ANDROID)
  install(FILES ${UMFCORE_HEADERS} DESTINATION "android-lib/jni/include/umf")
  install(FILES ${VMDATASOURCE_HEADERS} DESTINATION "android-lib/jni/include/umf")
else()
  install(FILES ${UMFCORE_HEADERS} DESTINATION "include/umf")
  install(FILES ${VMDATASOURCE_HEADERS} DESTINATION "include/umf")
endif()

if(ANDROID)
  install(DIRECTORY "umfcore/android-lib" DESTINATION ".")
  install(DIRECTORY "umfcore/java/src" DESTINATION "android-lib")
endif()

if(CODE_COVERAGE)
    append_target_property(${UMF_LIBRARY_NAME} LINK_FLAGS "--coverage")
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
    append_target_property(${UMF_LIBRARY_NAME} LINK_FLAGS "-Wl,-z,muldefs")
endif()

# tests
if(BUILD_TESTS)
    add_executable(${VMDATASOURCE_TEST_EXECUTABLE} ${VMDATASOURCE_TESTS})
    target_link_libraries(${VMDATASOURCE_TEST_EXECUTABLE} gtest umf)
    set_target_properties(${VMDATASOURCE_TEST_EXECUTABLE} PROPERTIES FOLDER "tests")

    add_executable(${UMFCORE_TEST_EXECUTABLE} ${UMFCORE_TESTS})
    target_link_libraries(${UMFCORE_TEST_EXECUTABLE} gtest umf)
    set_target_properties(${UMFCORE_TEST_EXECUTABLE} PROPERTIES FOLDER "tests")

    if(CODE_COVERAGE)
        append_target_property(${UMFCORE_TEST_EXECUTABLE} LINK_FLAGS "--coverage")
        append_target_property(${VMDATASOURCE_TEST_EXECUTABLE} LINK_FLAGS "--coverage")
    endif()

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
        append_target_property(${UMFCORE_TEST_EXECUTABLE} LINK_FLAGS "-Wl,-z,muldefs")
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
