cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(samples)

option(BUILD_QT_SAMPLES "Whether or not to build the Qt-based samples" OFF)

add_subdirectory(checksum-utility)
add_subdirectory(metadata-read-write)
add_subdirectory(metadata-statistics)
add_subdirectory(metadata-manipulation)
add_subdirectory(std-schema)
add_subdirectory(metadata-schema)
add_subdirectory(compression)
add_subdirectory(encryption)

if(BUILD_QT_SAMPLES)
  add_subdirectory(qt/unicode)
  add_subdirectory(qt/ski-resort)
  add_subdirectory(qt/photo-book)
endif()
