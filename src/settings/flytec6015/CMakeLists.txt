include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/common/window
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/database/flytec
                    ${CMAKE_SOURCE_DIR}/src/images
                    ${CMAKE_CURRENT_BINARY_DIR})

set(cfgflytec6015_SRCS AcousticFrame6015Impl.cpp
                       CorrFrame6015Impl.cpp
                       DeviceFrame6015Impl.cpp
                       DisplayFrame6015Impl.cpp
                       Frame6015.cpp
                       GPSFrame6015Impl.cpp
                       IFlytec6015Config.cpp
                       MainFrame6015Impl.cpp
                       MemoryFrame6015Impl.cpp
                       UnitFrame6015Impl.cpp
                       VarioFrame6015Impl.cpp
                       VelocityFrame6015Impl.cpp )

add_library(cfgflytec6015 ${cfgflytec6015_SRCS})
