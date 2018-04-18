CMAKE_MINIMUM_REQUIRED( VERSION 3.8 )
PROJECT( VideoTimeline CXX)

################################################################################
# Compiler and linker options  
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Include cmake modules
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
INCLUDE(cmake/GetGitRevisionDescription.cmake)
# Get git revision
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
STRING(SUBSTRING "${GIT_SHA1}" 0 12 GIT_REV)
IF(NOT GIT_SHA1)
    SET(GIT_REV "0")
ENDIF()

SET(META_PROJECT_NAME        "VideoTimeLine") 
SET(META_MAJOR_VERSION       "0")
SET(META_MINOR_VERSION       "0")
SET(META_PATCH_VERSION       "1")
SET(META_VERSION_REVISION    "${GIT_REV}") 
SET(META_CMAKE_INIT_SHA      "${GIT_SHA1}")
SET(META_NAME_VERSION        "Product: ${META_PROJECT_NAME} Commit: ${META_CMAKE_INIT_SHA}")
SET(META_VERSION_NUM         "Version: ${META_MAJOR_VERSION}.${META_MINOR_VERSION}.${META_PATCH_VERSION}")

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h)

################################################################################
# Sources and headers   
FIND_PACKAGE(Qt5 COMPONENTS Core Widgets Gui Qml REQUIRED) 

FILE(GLOB V1_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/v1/*.h")
FILE(GLOB V1_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/v1/*.cpp")  

FILE(GLOB V2_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/v2/*.h")
FILE(GLOB V2_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/v2/*.cpp")  

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)

################################################################################
# Compile target
ADD_EXECUTABLE(TimelineRuler ${V1_SOURCE_FILES} ${V1_HEADER_FILES} ${V1_RESOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/src/v1/resource/timeline.qrc)
ADD_EXECUTABLE(TimelineQML ${V2_RESOURCE_FILES} ${V2_HEADER_FILES} ${V2_SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/src/v2/resource/timeline.qrc)
# https://www.kdab.com/using-cmake-with-qt-5/
qt5_use_modules(TimelineRuler Core Widgets Gui) 
qt5_use_modules(TimelineQML Core Widgets Gui Qml) 