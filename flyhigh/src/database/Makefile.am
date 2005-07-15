INCLUDES = -I$(top_srcdir)/src/common/container \
	-I$(top_srcdir)/src/common/utils -I$(top_srcdir)/src/database/flytec -I$(top_srcdir)/src/database/garmin \
	-I$(top_srcdir)/src/database/sql $(all_includes)
METASOURCES = AUTO

SUBDIRS = sql flytec garmin


noinst_HEADERS = IDataBase.h IGPSDevice.h ISql.h IFlyHighRC.h
lib_LIBRARIES =  libdatabase.a
libdatabase_a_SOURCES = IDataBase.cpp IGPSDevice.cpp ISql.cpp IFlyHighRC.cpp
