include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_CURRENT_BINARY_DIR} )

set(cfgflyhigh_SRCS DeviceConnectionWidget.cpp
                     DeviceConnectionParameters.cpp
                     FlyHighSettingsDlg.cpp
                     FoldersWidgetImpl.cpp
                     IPilotInfoFrame.cpp
                     PilotInfoFrameImpl.cpp)

set(cfgflyhigh_HDRS DeviceConnectionWidget.h
                     FoldersWidgetImpl.h
                     PilotInfoFrameImpl.h)

add_library(cfgflyhigh ${cfgflyhigh_SRCS})
