include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/settings/device
                    ${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR} )

set(cfgdatabase_SRCS CredentialsDlg.cpp
                     DatabaseWidget.cpp
                     FlyHighRCFrameImpl.cpp
                     IFlyHighRCFrame.cpp)

set(cfgdatabase_HDRS DatabaseWidget.h
                     FlyHighRCFrameImpl.h)

file (GLOB cfgdatabase_UI *.ui)

QT4_WRAP_CPP(cfgdatabase_SRCS ${cfgdatabase_HDRS})

QT4_WRAP_UI( cfgdatabase_UI_HDRS ${cfgdatabase_UI} )

add_library(cfgdatabase ${cfgdatabase_SRCS} ${cfgdatabase_HDRS} ${cfgdatabase_UI_HDRS} )
