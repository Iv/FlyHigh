INCLUDES = -I$(top_srcdir)/src/common/container -I$(top_srcdir)/src/common/form \
	-I$(top_srcdir)/src/database -I$(top_srcdir)/src/database/flytec -I$(top_srcdir)/src/database/sql \
	-I$(top_srcdir)/src/images $(all_includes)
METASOURCES = AUTO
lib_LIBRARIES =  libflytec.a
libflytec_a_SOURCES = AcousticFrame.ui CorrFrame.ui DeviceFrame.ui \
		DisplayFrame.ui FlyCalcFrame.ui GPSFrame.ui MemoryFrame.ui PolareFrame.ui UnitFrame.ui \
		VarioFrame.ui VelocityFrame.ui VelocityFrameImpl.cpp VarioFrameImpl.cpp \
		UnitFrameImpl.cpp PolareFrameImpl.cpp MemoryFrameImpl.cpp GPSFrameImpl.cpp \
		FlyCalcFrameImpl.cpp DisplayFrameImpl.cpp CorrFrameImpl.cpp AcousticFrameImpl.cpp \
		DeviceFrameImpl.cpp MainFrameImpl.cpp MainFrame.ui Frame.cpp IFlytecConfig.cpp SmsFrame.ui \
	SmsFrameImpl.cpp


noinst_HEADERS = AcousticFrameImpl.h CorrFrameImpl.h DeviceFrameImpl.h DisplayFrameImpl.h FlyCalcFrameImpl.h Frame.h GPSFrameImpl.h MainFrameImpl.h MemoryFrameImpl.h PolareFrameImpl.h UnitFrameImpl.h VarioFrameImpl.h IFlytecConfig.h
