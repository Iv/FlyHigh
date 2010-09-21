include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES} )

set(images_RC images.qrc)

qt4_add_resources( images_RC_SRCS ${images_RC} )

add_library(images ${images_RC_SRCS})

########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = $(all_includes)
#METASOURCES = AUTO
#lib_LIBRARIES =  libimages.a
#
#
#libimages_a_SOURCES = Images.cpp
#noinst_HEADERS =  Images.h
