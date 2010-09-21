include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR} )

set(cfgdatabase_SRCS FlyHighRCFrameImpl.cpp
                     IFlyHighRCFrame.cpp )

set(cfgdatabase_HDRS FlyHighRCFrameImpl.h)

file (GLOB cfgdatabase_UI *.ui)

QT4_WRAP_CPP(cfgdatabase_SRCS ${cfgdatabase_HDRS})

QT4_WRAP_UI( cfgdatabase_UI_HDRS ${cfgdatabase_UI} )

add_library(cfgdatabase ${cfgdatabase_SRCS} ${cfgdatabase_HDRS} ${cfgdatabase_UI_HDRS} )

########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/src/common/container -I$(top_srcdir)/src/common/form \
#	-I$(top_srcdir)/src/database $(all_includes)
#METASOURCES = AUTO
#lib_LIBRARIES = libdatabase.a
#libdatabase_a_SOURCES = FlyHighRCFrame.ui FlyHighRCFrameImpl.cpp \
#	IFlyHighRCFrame.cpp
#noinst_HEADERS = FlyHighRCFrameImpl.h IFlyHighRCFrame.h
