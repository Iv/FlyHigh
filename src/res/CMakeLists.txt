include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES} )

set(res_RC res.qrc)

qt4_add_resources( res_RC_SRCS ${res_RC} )

add_library(res ${res_RC_SRCS})

########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = $(all_includes)
#METASOURCES = AUTO
#lib_LIBRARIES =  libres.a
#
#
#libres_a_SOURCES = Images.cpp
#noinst_HEADERS =  Images.h
