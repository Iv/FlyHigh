include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${QT_INCLUDES}
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

set(cfgflyhigh_UIS FoldersWidget.ui
                    PilotInfoFrame.ui)

QT4_WRAP_CPP(cfgflyhigh_SRCS ${cfgflyhigh_HDRS})

QT4_WRAP_UI( cfgflyhigh_UI_HDRS ${cfgflyhigh_UIS} )

add_library(cfgflyhigh ${cfgflyhigh_SRCS} ${cfgflyhigh_HDRS} ${cfgflyhigh_UI_HDRS} )
