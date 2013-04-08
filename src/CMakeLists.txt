include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/src/analysis
                    ${CMAKE_SOURCE_DIR}/src/common
                    ${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/elevation
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/common/webmap
                    ${CMAKE_SOURCE_DIR}/src/common/window
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/preparation
                    ${CMAKE_SOURCE_DIR}/src/settings
                    ${CMAKE_SOURCE_DIR}/src/settings/flyhigh
                    ${CMAKE_SOURCE_DIR}/src/settings/flytec
                    ${CMAKE_SOURCE_DIR}/src/settings/flytec6015
                    ${CMAKE_SOURCE_DIR}/src/tools/dbmigrator
                    ${QT_INCLUDES} )

add_subdirectory(analysis)
add_subdirectory(common)
add_subdirectory(database)
add_subdirectory(preparation)
add_subdirectory(res)
add_subdirectory(settings)
add_subdirectory(tools)

set(flyhigh_SRCS main.cpp
                 MainWindow.cpp)

set(flyhigh_HDRS MainWindow.h)

QT4_WRAP_CPP(flyhigh_SRCS ${flyhigh_HDRS})

if(WIN32)
 # no console window for windoze builds
 add_executable(flyhigh WIN32 ${flyhigh_SRCS} ${flyhigh_HDRS})
else(WIN32)
 add_executable(flyhigh ${flyhigh_SRCS} ${flyhigh_HDRS})
endif(WIN32)

target_link_libraries(flyhigh
	analysis
  preparation
	cfgflytec
	cfgflytec6015
	dbmigrator
	cfgdatabase
	window
	database
	dbflytec
	dbgarmin
	dbsql
	container
	elevation
	form
	map
	webmap
	utils
	res
	qextserialport
	${QT_LIBRARIES})

# install executable
install(TARGETS flyhigh DESTINATION ${FLYHIGH_BIN_DIR} )

# desktop file
install(FILES flyhigh.desktop DESTINATION ${FLYHIGH_APPLNK_DIR})

