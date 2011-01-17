cmake_minimum_required(VERSION 2.6)

project(flyhigh)

# at least Qt 4.3 is required
find_package(Qt4 4.3.0 COMPONENTS QtCore QtGui QtXml QtWebKit REQUIRED)

set(QT_USE_QT3SUPPORT TRUE)
set(QT_USE_QTWEBKIT TRUE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)
add_definitions(-DCOMPILE_USE_QT4)
add_definitions(-g -O0)

include(${QT_USE_FILE})
include_directories(${CMAKE_SOURCE_DIR} ${QT_INCLUDES})

add_subdirectory(src)
