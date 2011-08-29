include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${QT_INCLUDES})

set(webmap_SRCS WebMap.cpp
             WebMapAirSpaceView.cpp
             WebMapFlightView.cpp
             WebMapRouteView.cpp
             WebMapWayPointView.cpp
             PolyLineEncoder.cpp)

set(webmap_HDRS WebMap.h
             WebMapAirSpaceView.h
             WebMapFlightView.h
             WebMapWayPointView.h
             WebMapRouteView.h)

QT4_WRAP_CPP(webmap_SRCS ${webmap_HDRS})

add_library(webmap ${webmap_SRCS} ${webmap_HDRS})
