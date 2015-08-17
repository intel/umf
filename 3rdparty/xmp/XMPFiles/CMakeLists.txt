if(IOS)
    message(STATUS "O3->O1")
    foreach(flags CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_RELEASE)
      string(REPLACE "-O3" "-O1" ${flags} "${${flags}}")
    endforeach()
    foreach(flags CMAKE_C_FLAGS CMAKE_C_FLAGS_RELEASE)
      string(REPLACE "-O3" "-O1" ${flags} "${${flags}}")
    endforeach()
endif()

file(GLOB SOURCE
    "source/*.cpp"
    "source/*.hpp"
)

file(GLOB SOURCE_FileHandlers
    "source/FileHandlers/*.cpp"
    "source/FileHandlers/*.hpp"
)

file(GLOB SOURCE_FormatSupport
    "source/FormatSupport/*.cpp"
    "source/FormatSupport/*.hpp"
    "source/FormatSupport/*/*.h"
    "source/FormatSupport/*/*.hpp"
    "source/FormatSupport/*/*.cpp"
)

file(GLOB SOURCE_NativeMetadataSupport
    "source/NativeMetadataSupport/*.cpp"
    "source/NativeMetadataSupport/*.hpp"
)

file(GLOB SOURCE_PluginHandler
    "source/PluginHandler/*.cpp"
    "source/PluginHandler/*.hpp"
)

file(GLOB SOURCE_ThirdParty
    "../third-party/zlib/*.c"
    "../third-party/zlib/*.cpp"
    "../third-party/zlib/*.h"
)

source_group(PluginHandler FILES ${SOURCE_PluginHandler})
source_group(NativeMetadataSupport FILES ${SOURCE_NativeMetadataSupport})
source_group(FormatSupport FILES ${SOURCE_FormatSupport})
source_group(FileHandlers FILES ${SOURCE_FileHandlers})
source_group(ThirdParty FILES ${SOURCE_ThirdParty})

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/source/config.hpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/source/PluginHandler/config.hpp"
  )
include_directories(${CMAKE_CURRENT_BINARY_DIR}/source/PluginHandler)

if(WIN32)
    list(REMOVE_ITEM SOURCE_PluginHandler ${CMAKE_CURRENT_SOURCE_DIR}/source/PluginHandler/OS_Utils_Linux.cpp)
    list(REMOVE_ITEM SOURCE_PluginHandler ${CMAKE_CURRENT_SOURCE_DIR}/source/PluginHandler/OS_Utils_Mac.cpp)
elseif(UNIX AND NOT APPLE OR IOS)
    list(REMOVE_ITEM SOURCE_PluginHandler ${CMAKE_CURRENT_SOURCE_DIR}/source/PluginHandler/OS_Utils_WIN.cpp)
    list(REMOVE_ITEM SOURCE_PluginHandler ${CMAKE_CURRENT_SOURCE_DIR}/source/PluginHandler/OS_Utils_Mac.cpp)
elseif(APPLE)
    list(REMOVE_ITEM SOURCE_PluginHandler ${CMAKE_CURRENT_SOURCE_DIR}/source/PluginHandler/OS_Utils_WIN.cpp)
    list(REMOVE_ITEM SOURCE_PluginHandler ${CMAKE_CURRENT_SOURCE_DIR}/source/PluginHandler/OS_Utils_Linux.cpp)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
endif()

add_library(xmpfile OBJECT
    ${SOURCE} 
    ${SOURCE_FileHandlers}
    ${SOURCE_FormatSupport}
    ${SOURCE_NativeMetadataSupport}
    ${SOURCE_PluginHandler}
    ${SOURCE_ThirdParty})
