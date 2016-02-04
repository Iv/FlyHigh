set(elevation_SRCS Elevation.cpp
               ElevationDef.cpp
               ElevationSet.cpp
               ElevationSetList.cpp
               ElevationTile.cpp
               ElevationTileList.cpp
               SrtmReader.cpp )

add_library(elevation ${elevation_SRCS})
