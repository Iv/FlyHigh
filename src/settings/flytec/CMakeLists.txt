include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/common/window
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/database/flytec
                    ${CMAKE_SOURCE_DIR}/src/database/sql
                    ${CMAKE_SOURCE_DIR}/src/images
                    ${CMAKE_CURRENT_BINARY_DIR})

set(cfgflytec_SRCS VelocityFrameImpl.cpp
                   VarioFrameImpl.cpp
                   UnitFrameImpl.cpp
                   PolareFrameImpl.cpp
                   MemoryFrameImpl.cpp
                   GPSFrameImpl.cpp
                   FlyCalcFrameImpl.cpp
                   DisplayFrameImpl.cpp
                   CorrFrameImpl.cpp
                   AcousticFrameImpl.cpp
                   DeviceFrameImpl.cpp
                   MainFrameImpl.cpp
                   Frame.cpp
                   IFlytecConfig.cpp
                   SmsFrameImpl.cpp )

add_library(cfgflytec ${cfgflytec_SRCS})
