include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${QT_INCLUDES} )

set(dbsql_SRCS DataBaseSub.cpp
               Flights.cpp
               Gliders.cpp
               WayPoints.cpp
               Routes.cpp
               Servicings.cpp
               AirSpaces.cpp
               Upgrade.cpp
							 Migrator.cpp
							 Pilots.cpp
							 QueryStore.cpp)

set(dbsql_HDRS Migrator.h)

QT4_WRAP_CPP(dbsql_SRCS ${dbsql_HDRS})

add_library(dbsql ${dbsql_SRCS} ${dbsql_HDRS})
