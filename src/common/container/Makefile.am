INCLUDES = -I$(top_srcdir)/src/configuration/flytec \
	-I$(top_srcdir)/src/configuration/interface -I$(top_srcdir)/src/database $(all_includes)
METASOURCES = AUTO
lib_LIBRARIES =  libcontainer.a
libcontainer_a_SOURCES = Flight.cpp Glider.cpp Route.cpp WayPoint.cpp \
	Servicing.cpp AirSpace.cpp
noinst_HEADERS = Route.h ContainerDef.h Servicing.h AirSpace.h
