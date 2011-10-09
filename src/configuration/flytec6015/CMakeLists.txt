include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/database/flytec
                    ${CMAKE_SOURCE_DIR}/src/images
                    ${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR} )

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

set(cfgflytec6015_HDRS AcousticFrame6015Impl.h
                       CorrFrame6015Impl.h
                       DeviceFrame6015Impl.h
                       DisplayFrame6015Impl.h
                       GPSFrame6015Impl.h
                       MainFrame6015Impl.h
                       MemoryFrame6015Impl.h
                       UnitFrame6015Impl.h
                       VarioFrame6015Impl.h
                       VelocityFrame6015Impl.h )

set(cfgflytec6015_UIS
	AcousticFrame6015.ui
	CorrFrame6015.ui
	DeviceFrame6015.ui
	DisplayFrame6015.ui
	GPSFrame6015.ui
	MainFrame6015.ui
	MemoryFrame6015.ui
	UnitFrame6015.ui
	VarioFrame6015.ui
	VelocityFrame6015.ui)

QT4_WRAP_CPP(cfgflytec6015_SRCS ${cfgflytec6015_HDRS})

QT4_WRAP_UI( cfgflytec6015_UI_HDRS ${cfgflytec6015_UIS} )

add_library(cfgflytec6015 ${cfgflytec6015_SRCS} ${cfgflytec6015_HDRS} ${cfgflytec6015_UI_HDRS})
