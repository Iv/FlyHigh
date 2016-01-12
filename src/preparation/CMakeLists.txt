include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/common/webmap
                    ${CMAKE_SOURCE_DIR}/src/common/window
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/images)

set(preparation_SRCS AirSpaceView.cpp
                     AirSpaceWindow.cpp
                     OpenAirFileParser.cpp
                     RouteWindow.cpp
                     WayPointWindow.cpp
                     WptFileParser.cpp)

add_library(preparation ${preparation_SRCS})
