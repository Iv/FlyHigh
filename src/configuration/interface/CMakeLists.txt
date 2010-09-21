include_directories(${CMAKE_SOURCE_DIR}/src/database
                    ${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR} )

set(cfginterface_SRCS PortFrameImpl.cpp
                      IPortFrame.cpp )

set(cfginterface_HDRS PortFrameImpl.h)

set(cfginterface_UIS PortFrame.ui)

QT4_WRAP_CPP(cfginterface_SRCS ${cfginterface_HDRS})

QT4_WRAP_UI( cfginterface_UI_HDRS ${cfginterface_UIS} )

add_library(cfginterface ${cfginterface_SRCS} ${cfginterface_HDRS} ${cfginterface_UI_HDRS})


########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/src/database $(all_includes)
#METASOURCES = AUTO
#lib_LIBRARIES =  libinterface.a
#libinterface_a_SOURCES = PortFrame.ui PortFrameImpl.cpp IPortFrame.cpp
#
#noinst_HEADERS = PortFrameImpl.h IPortFrame.h
