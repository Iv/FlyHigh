include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${QT_INCLUDES})

set(webmap_SRCS WebMap.cpp
             WebMapAirSpace.cpp
             WebMapAirSpaceView.cpp
             WebMapFlight.cpp
             WebMapFlightView.cpp
             WebMapRoute.cpp
             WebMapRouteView.cpp
             WebMapWayPoint.cpp
             WebMapWayPointView.cpp
             PolyLineEncoder.cpp)

set(webmap_HDRS WebMap.h
             WebMapAirSpace.h
             WebMapAirSpaceView.h
             WebMapFlight.h
             WebMapFlightView.h
             WebMapWayPoint.h
             WebMapWayPointView.h
             WebMapRoute.h
             WebMapRouteView.h)

QT4_WRAP_CPP(webmap_SRCS ${webmap_HDRS})

add_library(webmap ${webmap_SRCS} ${webmap_HDRS})
