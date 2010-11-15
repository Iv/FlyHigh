include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${QT_INCLUDES})

set(webmap_SRCS WebMapFaiSector.cpp
             WebMapLeg.cpp
             WebMap.cpp
             WebMapOverlay.cpp
             WebMapItem.cpp
             WebMapScene.cpp
             WebMapWidget.cpp
             WebMapTurnPoint.cpp
             WebMapTrack.cpp
             PolyLineEncoder.cpp)

set(webmap_HDRS WebMap.h
             WebMapScene.h
             WebMapTurnPoint.h)

QT4_WRAP_CPP(webmap_SRCS ${webmap_HDRS})

add_library(webmap ${webmap_SRCS} ${webmap_HDRS})
