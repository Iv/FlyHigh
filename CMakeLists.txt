cmake_minimum_required(VERSION 2.6)

project(flyhigh)

find_package(Qt4 REQUIRED)

set(QT_USE_QT3SUPPORT TRUE)

#set(CMAKE_VERBOSE_MAKEFILE TRUE)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)
add_definitions(-DCOMPILE_USE_QT4)
add_definitions(-g -O0)

include(${QT_USE_FILE})
include_directories(${CMAKE_SOURCE_DIR} ${QT_INCLUDES})

#include(KDE4Defaults)

#include(MacroLibrary)

#include(ConvenienceLibs.cmake)

#include(ManualStuff.cmake)

#include(ConfigureChecks.cmake)

include_directories(${QT_INCLUDES} )

add_subdirectory(src)

message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}: skipped subdir $(TOPSUBDIRS)")

########### install files ###############




#original Makefile.am contents follow:

#SUBDIRS = $(TOPSUBDIRS)
#
#$(top_srcdir)/configure.in: configure.in.in $(top_srcdir)/subdirs
#	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.in ;
#
#$(top_srcdir)/subdirs:
#	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common subdirs
#
#$(top_srcdir)/acinclude.m4: $(top_srcdir)/admin/acinclude.m4.in $(top_srcdir)/admin/libtool.m4.in
#	@cd $(top_srcdir) && cat admin/acinclude.m4.in admin/libtool.m4.in > acinclude.m4
#
#MAINTAINERCLEANFILES = subdirs configure.in acinclude.m4 configure.files 
#
#package-messages:
#	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common package-messages
#	$(MAKE) -C po merge
#
#EXTRA_DIST = admin COPYING configure.in.in
#
#dist-hook:
#	cd $(top_distdir) && perl admin/am_edit -padmin
#	cd $(top_distdir) && $(MAKE) -f admin/Makefile.common subdirs
