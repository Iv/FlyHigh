include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/database/flytec
                    ${CMAKE_SOURCE_DIR}/src/database/garmin
                    ${CMAKE_SOURCE_DIR}/src/database/sql
                    ${CMAKE_SOURCE_DIR}/src/database/flytec6015
                    ${QT_INCLUDES} )

add_subdirectory(flytec)
add_subdirectory(garmin)
add_subdirectory(sql)
add_subdirectory(flytec6015)

set(database_SRCS IDataBase.cpp
                  IGPSDevice.cpp
                  ISql.cpp
                  IFlyHighRC.cpp )

set(database_HDRS IDataBase.h)

QT4_WRAP_CPP(database_SRCS ${database_HDRS})

add_library(database ${database_SRCS})

target_link_libraries(database dbflytec dbgarmin dbsql dbflytec6015)

########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/src/common/container \
#	-I$(top_srcdir)/src/common/utils -I$(top_srcdir)/src/database/flytec -I$(top_srcdir)/src/database/garmin \
#	-I$(top_srcdir)/src/database/sql -I$(top_srcdir)/src/database/flytec6015 $(all_includes)
#METASOURCES = AUTO
#
#SUBDIRS = flytec garmin sql flytec6015
#
#
#noinst_HEADERS = IDataBase.h IGPSDevice.h ISql.h IFlyHighRC.h
#lib_LIBRARIES =  libdatabase.a
#libdatabase_a_SOURCES = IDataBase.cpp IGPSDevice.cpp ISql.cpp IFlyHighRC.cpp
