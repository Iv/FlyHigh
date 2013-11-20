include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/elevation
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/common/map
                    ${CMAKE_SOURCE_DIR}/src/common/qextmedia
                    ${CMAKE_SOURCE_DIR}/src/common/window
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/images
                    ${QT_INCLUDES} )

set(analysis_SRCS FlightExpWindow.cpp
                  FlightWindow.cpp
                  GliderWindow.cpp
                  IGCFileParser.cpp
                  KmlWriter.cpp
                  OLCOptimizer.cpp
                  OLCWebForm.cpp
                  ServicingWindow.cpp)

set(analysis_HDRS FlightExpWindow.h
                  FlightWindow.h
                  GliderWindow.h
                  OLCOptimizer.h
                  ServicingWindow.h)

QT4_WRAP_CPP(analysis_SRCS ${analysis_HDRS})

add_library(analysis ${analysis_SRCS} ${analysis_HDRS})
