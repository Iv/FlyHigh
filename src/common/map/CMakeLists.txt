include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/map
                    ${CMAKE_SOURCE_DIR}/src/common/window)

set(map_SRCS Map.cpp
             MapTile.cpp
             MapView.cpp
             MapWidget.cpp
             SwissGrid.cpp
             SwissMap100.cpp
)

add_library(map ${map_SRCS})
