include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES} )

set(device_SRCS DeviceConnectionWidget.cpp
                DeviceConnectionParameters.cpp)

set(device_HDRS DeviceConnectionWidget.h)

QT4_WRAP_CPP(device_SRCS ${device_HDRS})


add_library(device ${device_SRCS})
