include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/elevation
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/common/map
                    ${CMAKE_SOURCE_DIR}/src/common/qextmedia
                    ${CMAKE_SOURCE_DIR}/src/common/window
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/images)

set(analysis_SRCS FlightExpWindow.cpp
                  FlightWindow.cpp
                  GliderWindow.cpp
                  IGCFileParser.cpp
                  KmlWriter.cpp
                  OLCOptimizer.cpp
                  ServicingWindow.cpp)

add_library(analysis ${analysis_SRCS})
