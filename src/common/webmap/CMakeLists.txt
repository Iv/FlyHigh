include_directories(${CMAKE_SOURCE_DIR}/src/common/container)

set(webmap_SRCS
  WebMap.cpp
  WebMapAirSpace.cpp
  WebMapAirSpaceView.cpp
  WebMapFlight.cpp
  WebMapFlightView.cpp
  WebMapRoute.cpp
  WebMapRouteView.cpp
  WebMapWayPoint.cpp
  WebMapWayPointView.cpp
  PolyLineEncoder.cpp)

add_library(webmap ${webmap_SRCS})
