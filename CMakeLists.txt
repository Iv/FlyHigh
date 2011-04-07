cmake_minimum_required(VERSION 2.6)

project(flyhigh)

# at least Qt 4.3 is required.
find_package(Qt4 4.3.0 COMPONENTS QtCore QtGui QtXml QtWebKit QtNetwork QtSql REQUIRED)

set(QT_USE_QT3SUPPORT TRUE)
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


add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS -DCOMPILE_USE_QT4)
add_definitions(-Wall)

include(${QT_USE_FILE})
include_directories(${CMAKE_SOURCE_DIR} ${QT_INCLUDES})

add_subdirectory(src)
add_subdirectory(tools)


# install files
install(FILES AUTHORS DESTINATION ${FLYHIGH_DOC_DIR})
install(FILES COPYING DESTINATION ${FLYHIGH_DOC_DIR})
install(FILES INSTALL DESTINATION ${FLYHIGH_DOC_DIR})
install(FILES NEWS DESTINATION ${FLYHIGH_DOC_DIR})
install(FILES README DESTINATION ${FLYHIGH_DOC_DIR})
install(FILES TODO DESTINATION ${FLYHIGH_DOC_DIR})
