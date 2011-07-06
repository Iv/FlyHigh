include(${QT_USE_FILE})

include_directories(${QT_INCLUDES})

IF(WIN32)
	set(qextserialport_SRCS
		win_qextserialport.cpp
#		qextserialenumerator_win.cpp
		qextserialport.cpp)
ELSE(WIN32) # unix systems
	set(qextserialport_SRCS
		posix_qextserialport.cpp
		qextserialenumerator_unix.cpp
		qextserialport.cpp)
ENDIF(WIN32)

set(qextserialport_HDRS
	qextserialenumerator.h
	qextserialport.h)

QT4_WRAP_CPP(qextserialport_SRCS ${qextserialport_HDRS})

add_library(qextserialport ${qextserialport_SRCS} ${qextserialport_HDRS})
