# set the include path for X, qt and KDE
INCLUDES = -I$(top_srcdir)/src/analysis -I$(top_srcdir)/src/common -I$(top_srcdir)/src/common/container -I$(top_srcdir)/src/common/form -I$(top_srcdir)/src/common/utils -I$(top_srcdir)/src/configuration/flytec -I$(top_srcdir)/src/configuration/interface -I$(top_srcdir)/src/database -I$(top_srcdir)/src/images -I$(top_srcdir)/src/preparation $(all_includes)

# these are the headers for your project


# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/flyhigh.pot

KDE_ICON = flyhigh

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = flyhigh

# the application source, library search path, and link libraries

flyhigh_LDFLAGS = $(KDE_RPATH) $(all_libraries)
flyhigh_LDADD = $(top_builddir)/src/preparation/libpreparation.a $(top_builddir)/src/analysis/libanalysis.a $(top_builddir)/src/common/form/libform.a $(top_builddir)/src/configuration/interface/libinterface.a $(top_builddir)/src/images/libimages.a $(top_builddir)/src/configuration/flytec/libflytec.a $(top_builddir)/src/database/libdatabase.a $(top_builddir)/src/database/sql/libsql.a $(top_builddir)/src/database/flytec/libflytec.a $(top_builddir)/src/common/container/libcontainer.a $(top_builddir)/src/common/utils/libutils.a $(LIB_KDEUI)

# this is where the desktop file will go 
shelldesktopdir = $(kde_appsdir)/Utilities
shelldesktop_DATA = flyhigh.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir = $(kde_datadir)/flyhigh
shellrc_DATA = flyhighui.rc

SUBDIRS = configuration common database analysis images preparation
flyhigh_SOURCES = main.cpp MainWindow.h MainWindow.cpp
kde_icon_KDEICON =  5020.png document.xpm polare.png

