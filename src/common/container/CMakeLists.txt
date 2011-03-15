include(${QT_USE_FILE})

include_directories(${QT_INCLUDES} )

set(container_SRCS AirSpace.cpp
                   AirSpaceItem.cpp
                   AirSpaceList.cpp
                   BoundBox.cpp
                   Flight.cpp
                   FlightPointList.cpp
                   Glider.cpp
                   Pilot.cpp
                   Route.cpp
                   Servicing.cpp
                   WayPoint.cpp )

add_library(container ${container_SRCS})
