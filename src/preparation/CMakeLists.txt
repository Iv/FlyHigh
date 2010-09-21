include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/images
                    ${QT_INCLUDES} )

set(preparation_SRCS AirSpaceView.cpp
                     AirSpaceWindow.cpp
                     OpenAirFileParser.cpp
                     RouteWindow.cpp
                     WayPointWindow.cpp)

set(preparation_HDRS WayPointWindow.h
                     RouteWindow.h
                     AirSpaceWindow.h)

QT4_WRAP_CPP(preparation_SRCS ${preparation_HDRS})

add_library(preparation ${preparation_SRCS} ${preparation_HDRS})

########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/src/common/container -I$(top_srcdir)/src/common/form \
#	-I$(top_srcdir)/src/common/utils -I$(top_srcdir)/src/database -I$(top_srcdir)/src/images $(all_includes)
#METASOURCES = AUTO
#lib_LIBRARIES =  libpreparation.a
#noinst_HEADERS = WayPointWindow.h RouteWindow.h AirSpaceWindow.h \
#	OpenAirFileParser.h
#libpreparation_a_SOURCES = AirSpaceView.cpp AirSpaceWindow.cpp \
#	OpenAirFileParser.cpp RouteWindow.cpp WayPointWindow.cpp
