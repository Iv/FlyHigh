include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/configuration/flytec
                    ${CMAKE_SOURCE_DIR}/src/configuration/interface
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${QT_INCLUDES} )

add_subdirectory(form)
add_subdirectory(container)
add_subdirectory(utils)
add_subdirectory(map)
add_subdirectory(webmap)


########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/src/configuration/flytec \
#	-I$(top_srcdir)/src/configuration/interface -I$(top_srcdir)/src/database $(all_includes)
#METASOURCES = AUTO
#
#
#
#SUBDIRS = form container utils map
