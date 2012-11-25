include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/common/qextserialport
                    ${CMAKE_SOURCE_DIR}/src/database/flytec
                    ${CMAKE_SOURCE_DIR}/src/database/garmin
                    ${CMAKE_SOURCE_DIR}/src/database/sql
                    ${CMAKE_SOURCE_DIR}/src/settings/database
                    ${QT_INCLUDES} )

add_subdirectory(flytec)
add_subdirectory(garmin)
add_subdirectory(sql)

set(database_SRCS IDataBase.cpp
                  IGPSDevice.cpp
                  ISql.cpp
									IFlyHighRC.cpp
									DatabaseParameters.cpp)

set(database_HDRS IDataBase.h
                  ISql.h)

QT4_WRAP_CPP(database_SRCS ${database_HDRS})

add_library(database ${database_SRCS})

target_link_libraries(database dbflytec dbgarmin dbsql)
