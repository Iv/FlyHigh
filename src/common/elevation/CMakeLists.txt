include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES} )

set(elevation_SRCS Elevation.cpp
               ElevationDef.cpp
               ElevationSet.cpp
               ElevationSetList.cpp
               ElevationTile.cpp
               ElevationTileList.cpp
               SrtmReader.cpp )

set(elevation_HDRS Elevation.h
                   ElevationSet.h)

QT4_WRAP_CPP(elevation_SRCS ${elevation_HDRS})

add_library(elevation ${elevation_SRCS})
