include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${QT_INCLUDES})

set(webmap_SRCS WebMapFaiSector.cpp
             WebMapLeg.cpp
             WebMap.cpp
             WebMapOverlay.cpp
             WebMapOverlayItem.cpp
             WebMapOverlayScene.cpp
             WebMapWidget.cpp
             WebMapTurnPoint.cpp)

set(webmap_HDRS WebMap.h
             WebMapOverlayScene.h
             WebMapTurnPoint.h)

QT4_WRAP_CPP(webmap_SRCS ${webmap_HDRS})

add_library(webmap ${webmap_SRCS} ${webmap_HDRS})
