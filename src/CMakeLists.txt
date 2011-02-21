include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/src/analysis
                    ${CMAKE_SOURCE_DIR}/src/common
                    ${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/common/webmap
                    ${CMAKE_SOURCE_DIR}/src/configuration/database
                    ${CMAKE_SOURCE_DIR}/src/configuration/flytec
                    ${CMAKE_SOURCE_DIR}/src/configuration/interface
                    ${CMAKE_SOURCE_DIR}/src/configuration/flytec6015
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/preparation
                    ${QT_INCLUDES} )

add_subdirectory(analysis)
add_subdirectory(common)
add_subdirectory(configuration)
add_subdirectory(database)
add_subdirectory(res)
add_subdirectory(preparation)

set(flyhigh_SRCS main.cpp
                 MainWindow.cpp)

set(flyhigh_HDRS MainWindow.h)

QT4_WRAP_CPP(flyhigh_SRCS ${flyhigh_HDRS})

add_executable(flyhigh ${flyhigh_SRCS} ${flyhigh_HDRS})

target_link_libraries(flyhigh
	analysis
	cfgdatabase
	cfgflytec
	cfgflytec6015
	cfginterface
	database
	dbflytec
	dbflytec6015
	dbgarmin
	dbsql
	preparation
	container
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

