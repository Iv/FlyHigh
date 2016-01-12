cmake_minimum_required(VERSION 3.0.2)

project(flyhigh)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)

# at least Qt 5.3 is required.
find_package(Qt5Core 5.3.2 REQUIRED)
find_package(Qt5Gui 5.3.2 REQUIRED)
find_package(Qt5Network 5.3.2 REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5SerialPort REQUIRED)
find_package(Qt5Sql 5.3.2 REQUIRED)
find_package(Qt5Widgets 5.3.2 REQUIRED)
find_package(Qt5Xml 5.3.2 REQUIRED)
find_package(Qt5WebKit 5.3.2 REQUIRED)
find_package(Qt5WebKitWidgets 5.3.2 REQUIRED)

set(QT_USE_QTWEBKIT TRUE)

# default build type is Release
if(NOT CMAKE_BUILD_TYPE)
 set(CMAKE_BUILD_TYPE "Release")
endif()

message("Build type is: " ${CMAKE_BUILD_TYPE})

# verbose makefiles if doing a Debug build
# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#  set(CMAKE_VERBOSE_MAKEFILE TRUE)
# else()
#  set(CMAKE_VERBOSE_MAKEFILE FALSE)
# endif()

# set file locations for installation
# where to install binaries
if(NOT FLYHIGH_BIN_DIR)
 set(FLYHIGH_BIN_DIR "${CMAKE_INSTALL_PREFIX}/usr/bin")
endif()
# where to install desktop link
if(NOT FLYHIGH_APPLNK_DIR)
 set(FLYHIGH_APPLNK_DIR "${CMAKE_INSTALL_PREFIX}/usr/share/applications")
endif()
# where to install icons
if(NOT FLYHIGH_ICON_DIR)
 set(FLYHIGH_ICON_DIR "${CMAKE_INSTALL_PREFIX}/usr/share/pixmaps")
endif()
# where to install documentation
if(NOT FLYHIGH_DOC_DIR)
 set(FLYHIGH_DOC_DIR "${CMAKE_INSTALL_PREFIX}/usr/share/doc/flyhigh")
endif()

add_definitions(-Wall -fPIC -std=c++11 -msse2)

include(Functions.cmake) # custom functions
include_directories(${CMAKE_SOURCE_DIR}
)

add_subdirectory(src)
add_subdirectory(tools)

# install files
install(FILES AUTHORS DESTINATION ${FLYHIGH_DOC_DIR})
install(FILES COPYING DESTINATION ${FLYHIGH_DOC_DIR})
install(FILES INSTALL DESTINATION ${FLYHIGH_DOC_DIR})
install(FILES NEWS DESTINATION ${FLYHIGH_DOC_DIR})
install(FILES README DESTINATION ${FLYHIGH_DOC_DIR})
install(FILES TODO DESTINATION ${FLYHIGH_DOC_DIR})
