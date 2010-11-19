include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/common/map
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/images
                    ${QT_INCLUDES} )

set(analysis_SRCS FlightExpWindow.cpp
                  FlightWindow.cpp
                  GliderWindow.cpp
                  IGCFileParser.cpp
                  KmlWriter.cpp
                  OLCOptimizer.cpp
                  OLCWebForm.cpp
                  ServicingWindow.cpp
                  WebMapFlightView.cpp)

set(analysis_HDRS FlightExpWindow.h
                  FlightWindow.h
                  GliderWindow.h
                  OLCOptimizer.h
                  ServicingWindow.h
                  WebMapFlightView.h)

QT4_WRAP_CPP(analysis_SRCS ${analysis_HDRS})

add_library(analysis ${analysis_SRCS} ${analysis_HDRS})

########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/src/common/container -I$(top_srcdir)/src/common/form \
#	-I$(top_srcdir)/src/common/utils -I$(top_srcdir)/src/common/map -I$(top_srcdir)/src/database \
#	-I$(top_srcdir)/src/images $(all_includes)
#METASOURCES = AUTO
#lib_LIBRARIES =  libanalysis.a
#libanalysis_a_SOURCES = FlightExpWindow.cpp FlightPointList.cpp \
#	FlightWindow.cpp GliderWindow.cpp IGCFileParser.cpp KmlWriter.cpp OLCOptimizer.cpp \
#	OLCWebForm.cpp ServicingWindow.cpp
#
#
#
#
#noinst_HEADERS = FlightExpWindow.h FlightPointList.h FlightWindow.h \
#	GliderWindow.h IGCFileParser.h KmlWriter.h OLCOptimizer.h OLCWebForm.h \
#	ServicingWindow.h
