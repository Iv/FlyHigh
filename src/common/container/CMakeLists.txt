include(${QT_USE_FILE})

include_directories(${QT_INCLUDES} )

set(container_SRCS AirSpace.cpp
                   AirSpaceList.cpp
                   BoundBox.cpp
                   Flight.cpp
                   FlightPoint.cpp
                   FlightPointList.cpp
                   Glider.cpp
                   LatLng.cpp
                   OpenAir.cpp
                   OpenAirItem.cpp
                   Pilot.cpp
                   Photo.cpp
                   Route.cpp
                   Servicing.cpp
                   WayPoint.cpp )

add_library(container ${container_SRCS})
