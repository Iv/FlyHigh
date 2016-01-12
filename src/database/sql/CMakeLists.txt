include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/database)

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
               QueryExecutor.cpp
               QueryStore.cpp
               Query.cpp
               DatabaseUtilities.cpp
               Accounts.cpp)

add_library(dbsql ${dbsql_SRCS})

# This line fixes the strange linking problem on Windoze/mingw32. Don't ask...
target_link_libraries(dbsql ${QT_LIBRARIES})
