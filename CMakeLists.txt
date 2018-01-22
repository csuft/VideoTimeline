CMAKE_MINIMUM_REQUIRED( VERSION 3.8 )
PROJECT( VideoTimeline CXX)

################################################################################
# Compiler and linker options
ADD_DEFINITIONS(-Wall) 

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_CXX_STANDARD 11)  

# Include cmake modules
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
INCLUDE(cmake/GetGitRevisionDescription.cmake)
# Get git revision
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
STRING(SUBSTRING "${GIT_SHA1}" 0 12 GIT_REV)
IF(NOT GIT_SHA1)
    SET(GIT_REV "0")
ENDIF()

SET(META_PROJECT_NAME        "TimelineRuler") 
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
SET(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/timeline.qrc) 
FIND_PACKAGE(Qt5 COMPONENTS Core Widgets Gui REQUIRED) 

FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
FILE(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp") 

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/)

################################################################################
# Compile target
ADD_EXECUTABLE ( ${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCES})
# https://www.kdab.com/using-cmake-with-qt-5/
qt5_use_modules(${PROJECT_NAME} Core Widgets Gui) 