include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/src/analysis
                    ${CMAKE_SOURCE_DIR}/src/common
                    ${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/common/webmap
                    ${CMAKE_SOURCE_DIR}/src/configuration/database
                    ${CMAKE_SOURCE_DIR}/src/configuration/flytec
                    ${CMAKE_SOURCE_DIR}/src/configuration/interface
                    ${CMAKE_SOURCE_DIR}/src/configuration/flytec6015
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/preparation
                    ${QT_INCLUDES} )

add_subdirectory(analysis)
add_subdirectory(common)
add_subdirectory(configuration)
add_subdirectory(database)
add_subdirectory(res)
add_subdirectory(preparation)

set(flyhigh_SRCS main.cpp
                 MainWindow.cpp)

set(flyhigh_HDRS MainWindow.h)

QT4_WRAP_CPP(flyhigh_SRCS ${flyhigh_HDRS})

add_executable(flyhigh ${flyhigh_SRCS} ${flyhigh_HDRS})

target_link_libraries(flyhigh ${QT_LIBRARIES}
                 analysis
                 cfgdatabase
                 cfgflytec
                 cfgflytec6015
                 cfginterface
                 database
                 dbflytec
                 dbflytec6015
                 dbgarmin
                 dbsql
                 preparation
                 container
                 form
                 map
                 webmap
                 utils
                 res )


########### install files ###############

install(FILES  flyhighui.rc DESTINATION ${DATA_INSTALL_DIR}/flyhigh)
install(FILES  flyhigh.desktop DESTINATION ${APPLNK_INSTALL_DIR}/Utilities)



#original Makefile.am contents follow:

## set the include path for X, qt and KDE
#INCLUDES = -I$(top_srcdir)/src/analysis -I$(top_srcdir)/src/common \
#	-I$(top_srcdir)/src/common/container -I$(top_srcdir)/src/common/form -I$(top_srcdir)/src/common/utils \
#	-I$(top_srcdir)/src/configuration/database -I$(top_srcdir)/src/configuration/flytec \
#	-I$(top_srcdir)/src/configuration/interface -I$(top_srcdir)/src/configuration/flytec6015 \
#	-I$(top_srcdir)/src/database -I$(top_srcdir)/src/images -I$(top_srcdir)/src/preparation \
#	$(all_includes)
#
## these are the headers for your project
#
#
## let automoc handle all of the meta source files (moc)
#METASOURCES = AUTO
#
#messages: rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/flyhigh.pot
#
#KDE_ICON = flyhigh
#
##########################################################################
## APPLICATION SECTION
##########################################################################
## this is the program that gets installed.  it's name is used for all
## of the other Makefile.am variables
#bin_PROGRAMS = flyhigh
#
## the application source, library search path, and link libraries
#
#flyhigh_LDFLAGS = $(KDE_RPATH) $(all_libraries)
#flyhigh_LDADD = $(top_builddir)/src/analysis/libanalysis.a \
#	$(top_builddir)/src/configuration/database/libdatabase.a \
#	$(top_builddir)/src/configuration/flytec/libflytec.a \
#	$(top_builddir)/src/configuration/flytec6015/libflytec6015.a \
#	$(top_builddir)/src/configuration/interface/libinterface.a \
#	$(top_builddir)/src/database/libdatabase.a \
#	$(top_builddir)/src/database/flytec/libflytec.a \
#	$(top_builddir)/src/database/flytec6015/libflytec6015.a \
#	$(top_builddir)/src/database/garmin/libgarmin.a \
#	$(top_builddir)/src/database/sql/libsql.a \
#	$(top_builddir)/src/preparation/libpreparation.a  \
#	$(top_builddir)/src/common/container/libcontainer.a \
#	$(top_builddir)/src/common/form/libform.a \
#	$(top_builddir)/src/common/map/libmap.a \
#	$(top_builddir)/src/common/utils/libutils.a \
#	$(top_builddir)/src/images/libimages.a \
#	$(LIB_KDEUI)
#
## this is where the desktop file will go 
#shelldesktopdir = $(kde_appsdir)/Utilities
#shelldesktop_DATA = flyhigh.desktop
#
## this is where the shell's XML-GUI resource file goes
#shellrcdir = $(kde_datadir)/flyhigh
#shellrc_DATA = flyhighui.rc
#
#SUBDIRS = analysis common configuration database images preparation
#flyhigh_SOURCES = main.cpp MainWindow.h MainWindow.cpp
#kde_icon_KDEICON = 5020.png document.xpm polare.png
#
