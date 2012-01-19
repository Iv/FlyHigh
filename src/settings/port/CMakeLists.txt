include_directories(${CMAKE_SOURCE_DIR}/src/database
                    ${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR} )

set(cfgport_SRCS PortFrameImpl.cpp
                 IPortFrame.cpp )

set(cfgport_HDRS PortFrameImpl.h)

set(cfgport_UIS PortFrame.ui)

QT4_WRAP_CPP(cfgport_SRCS ${cfgport_HDRS})

QT4_WRAP_UI( cfgport_UI_HDRS ${cfgport_UIS} )

add_library(cfgport ${cfgport_SRCS} ${cfgport_HDRS} ${cfgport_UI_HDRS})
