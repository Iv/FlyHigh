include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/common/map
                    ${QT_INCLUDES})

set(map_SRCS Map.cpp
             MapTile.cpp
             MapView.cpp
             SwissGrid.cpp
             SwissMap100.cpp
)

set(map_HDRS Map.h)

QT4_WRAP_CPP(map_SRCS ${map_HDRS})

add_library(map ${map_SRCS} ${map_HDRS})

########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/src/common/container -I$(top_srcdir)/src/common/form \
#	$(all_includes)
#METASOURCES = AUTO
#lib_LIBRARIES = libmap.a
#libmap_a_SOURCES = Map.cpp Map.h MapTile.cpp MapTile.h MapView.cpp MapView.h \
#	SwissGrid.cpp SwissGrid.h SwissMap100.cpp SwissMap100.h
#
