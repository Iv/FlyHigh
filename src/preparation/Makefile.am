INCLUDES = -I$(top_srcdir)/src/common/container -I$(top_srcdir)/src/common/form \
	-I$(top_srcdir)/src/common/utils -I$(top_srcdir)/src/database -I$(top_srcdir)/src/images $(all_includes)
METASOURCES = AUTO
lib_LIBRARIES =  libpreparation.a
noinst_HEADERS = WayPointWindow.h RouteWindow.h AirSpaceWindow.h \
	OpenAirFileParser.h
libpreparation_a_SOURCES = AirSpaceView.cpp AirSpaceWindow.cpp \
	OpenAirFileParser.cpp RouteWindow.cpp WayPointWindow.cpp
