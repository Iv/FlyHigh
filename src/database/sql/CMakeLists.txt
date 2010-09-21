include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${QT_INCLUDES} )

set(dbsql_SRCS DataBaseSub.cpp
               Flights.cpp
               Gliders.cpp
               WayPoints.cpp
               Routes.cpp
               Servicings.cpp
               AirSpaces.cpp
               Upgrade.cpp
               Pilots.cpp)

add_library(dbsql ${dbsql_SRCS})

########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/src/common/container \
#	-I$(top_srcdir)/src/common/utils -I$(top_srcdir)/src/database $(all_includes)
#METASOURCES = AUTO
#lib_LIBRARIES =  libsql.a
#libsql_a_SOURCES = DataBaseSub.cpp Flights.cpp Gliders.cpp WayPoints.cpp \
#	Flights.h Gliders.h WayPoints.h DataBaseSub.h Routes.cpp Servicings.cpp \
#	AirSpaces.cpp Upgrade.cpp Pilots.cpp
#noinst_HEADERS = Routes.h Servicings.h Upgrade.h Pilots.h
