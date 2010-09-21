include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES} )

add_subdirectory(database)
add_subdirectory(flytec)
add_subdirectory(flytec6015)
add_subdirectory(interface)


########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = $(all_includes)
#METASOURCES = AUTO
#SUBDIRS = database flytec flytec6015 interface
