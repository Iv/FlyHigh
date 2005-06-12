INCLUDES = -I$(top_srcdir)/src/common/container \
	-I$(top_srcdir)/src/common/utils -I$(top_srcdir)/src/database $(all_includes)
METASOURCES = AUTO
lib_LIBRARIES =  libsql.a
libsql_a_SOURCES = DataBaseSub.cpp Flights.cpp Gliders.cpp WayPoints.cpp \
			Flights.h Gliders.h WayPoints.h DataBaseSub.h Routes.cpp Servicings.cpp \
		AirSpaces.cpp SqlSetup.cpp
noinst_HEADERS = Routes.h Servicings.h SqlSetup.h
