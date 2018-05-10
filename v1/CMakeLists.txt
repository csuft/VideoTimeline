CMAKE_MINIMUM_REQUIRED( VERSION 3.8 )
PROJECT(WidgetsBased CXX) 

################################################################################
# Compiler and linker options  
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

################################################################################
# Sources and headers   
FIND_PACKAGE(Qt5 COMPONENTS Core Widgets Gui REQUIRED) 

FILE(GLOB V1_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
FILE(GLOB V1_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")  

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

################################################################################
# Compile target
ADD_EXECUTABLE(${PROJECT_NAME} ${V1_SOURCE_FILES} ${V1_HEADER_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/resource/timeline.qrc)
# https://www.kdab.com/using-cmake-with-qt-5/
qt5_use_modules(${PROJECT_NAME} Core Widgets Gui) 