include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES} )

set(utils_SRCS Error.cpp
               GnuPlot.cpp
               Tokenizer.cpp )

add_library(utils ${utils_SRCS})

########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = $(all_includes)
#METASOURCES = AUTO
#lib_LIBRARIES =  libutils.a
#libutils_a_SOURCES = Error.cpp GnuPlot.cpp Tokenizer.cpp
#noinst_HEADERS = Error.h GnuPlot.h Tokenizer.h
