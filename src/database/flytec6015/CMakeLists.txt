include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${QT_INCLUDES} )

set(dbflytec6015_SRCS Device6015.cpp
                      Flytec6015.cpp
                      Protocol6015.cpp)

add_library(dbflytec6015 ${dbflytec6015_SRCS})

########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/src/common/container \
#	-I$(top_srcdir)/src/common/utils -I$(top_srcdir)/src/database $(all_includes)
#METASOURCES = AUTO
#lib_LIBRARIES = libflytec6015.a
#libflytec6015_a_SOURCES = Device6015.cpp Flytec6015.cpp Protocol6015.cpp
#noinst_HEADERS = Defs6015.h Protocol6015.h
