include(${QT_USE_FILE})

include_directories(${QT_INCLUDES} )

set(container_SRCS AirSpace.cpp
                   AirSpaceItem.cpp
                   BoundBox.cpp
                   Flight.cpp
                   FlightPointList.cpp
                   Glider.cpp
                   Pilot.cpp
                   Route.cpp
                   Servicing.cpp
                   WayPoint.cpp )

add_library(container ${container_SRCS})

########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = $(all_includes)
#METASOURCES = AUTO
#lib_LIBRARIES =  libcontainer.a
#libcontainer_a_SOURCES = AirSpace.cpp AirSpaceItem.cpp BoundBox.cpp Flight.cpp \
#	Glider.cpp Pilot.cpp Route.cpp Servicing.cpp WayPoint.cpp
#noinst_HEADERS = Route.h ContainerDef.h Servicing.h AirSpace.h AirSpaceItem.h \
#	Pilot.h
