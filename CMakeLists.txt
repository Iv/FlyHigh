cmake_minimum_required(VERSION 2.6)

project(flyhigh)

# at least Qt 4.3 is required
find_package(Qt4 4.3.0 COMPONENTS QtCore QtGui QtXml QtWebKit REQUIRED)

set(QT_USE_QT3SUPPORT TRUE)
set(QT_USE_QTWEBKIT TRUE)

# default build type is Debug
if( NOT CMAKE_BUILD_TYPE )
set(CMAKE_BUILD_TYPE "Debug")
endif()

message("Build type is: " ${CMAKE_BUILD_TYPE})

# verbose makefiles if doing a Debug build
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
set(CMAKE_VERBOSE_MAKEFILE TRUE)
else()
set(CMAKE_VERBOSE_MAKEFILE FALSE)
endif()

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)
add_definitions(-DCOMPILE_USE_QT4)

include(${QT_USE_FILE})
include_directories(${CMAKE_SOURCE_DIR} ${QT_INCLUDES})

add_subdirectory(src)

# remove build type from cache
unset( CMAKE_BUILD_TYPE CACHE)
