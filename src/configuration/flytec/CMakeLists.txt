include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/database/flytec
                    ${CMAKE_SOURCE_DIR}/src/database/sql
                    ${CMAKE_SOURCE_DIR}/src/images
                    ${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR} )

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

set(cfgflytec_HDRS AcousticFrameImpl.h
                   CorrFrameImpl.h
                   DeviceFrameImpl.h
                   DisplayFrameImpl.h
                   FlyCalcFrameImpl.h
                   GPSFrameImpl.h
                   MainFrameImpl.h
                   MemoryFrameImpl.h
                   PolareFrameImpl.h
                   UnitFrameImpl.h
                   VarioFrameImpl.h
                   SmsFrameImpl.h
                   VelocityFrameImpl.h)

set(cfgflytec_UIS
	AcousticFrame.ui
	CorrFrame.ui
	DeviceFrame.ui
	DisplayFrame.ui
	FlyCalcFrame.ui
	GPSFrame.ui
	MainFrame.ui
	MemoryFrame.ui
	PolareFrame.ui
	SmsFrame.ui
	UnitFrame.ui
	VarioFrame.ui
	VelocityFrame.ui)

QT4_WRAP_CPP(cfgflytec_SRCS ${cfgflytec_HDRS} )

QT4_WRAP_UI(cfgflytec_UI_HDRS ${cfgflytec_UIS} )

add_library(cfgflytec ${cfgflytec_SRCS} ${cfgflytec_HDRS} ${cfgflytec_UI_HDRS})


########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/src/common/container -I$(top_srcdir)/src/common/form \
#	-I$(top_srcdir)/src/database -I$(top_srcdir)/src/database/flytec -I$(top_srcdir)/src/database/sql \
#	-I$(top_srcdir)/src/images $(all_includes)
#METASOURCES = AUTO
#lib_LIBRARIES =  libflytec.a
#libflytec_a_SOURCES = AcousticFrame.ui CorrFrame.ui DeviceFrame.ui \
#		DisplayFrame.ui FlyCalcFrame.ui GPSFrame.ui MemoryFrame.ui PolareFrame.ui UnitFrame.ui \
#		VarioFrame.ui VelocityFrame.ui VelocityFrameImpl.cpp VarioFrameImpl.cpp \
#		UnitFrameImpl.cpp PolareFrameImpl.cpp MemoryFrameImpl.cpp GPSFrameImpl.cpp \
#		FlyCalcFrameImpl.cpp DisplayFrameImpl.cpp CorrFrameImpl.cpp AcousticFrameImpl.cpp \
#		DeviceFrameImpl.cpp MainFrameImpl.cpp MainFrame.ui Frame.cpp IFlytecConfig.cpp SmsFrame.ui \
#	SmsFrameImpl.cpp
#
#
#noinst_HEADERS = AcousticFrameImpl.h CorrFrameImpl.h DeviceFrameImpl.h DisplayFrameImpl.h FlyCalcFrameImpl.h Frame.h GPSFrameImpl.h MainFrameImpl.h MemoryFrameImpl.h PolareFrameImpl.h UnitFrameImpl.h VarioFrameImpl.h IFlytecConfig.h
