include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR})

set(form_SRCS AirSpaceFormImpl.cpp
              FlightFormImpl.cpp
              GliderFormImpl.cpp
              MDIWindow.cpp
              TableWindow.cpp
              WayPointFormImpl.cpp
              IWayPointForm.cpp
              IGliderForm.cpp
              IFlightForm.cpp
              RouteFormImpl.cpp
              IRouteForm.cpp
              ServicingFormImpl.cpp
              IServicingForm.cpp
              ProgressDlg.cpp)

set(form_HDRS FlightFormImpl.h
              GliderFormImpl.h
              MDIWindow.h
              TableWindow.h
              WayPointFormImpl.h
              RouteFormImpl.h
              ServicingFormImpl.h
              ProgressDlg.h)

set(form_UIS
	AirSpaceForm.ui
	FlightForm.ui
	GliderForm.ui
	RouteForm.ui
	ServicingForm.ui
	WayPointForm.ui)

QT4_WRAP_CPP(form_SRCS ${form_HDRS})

QT4_WRAP_UI(form_UI_HDRS ${form_UIS})

add_library(form ${form_SRCS} ${form_HDRS} ${form_UI_HDRS})

########### install files ###############




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/src/common/container \
#       -I$(top_srcdir)/src/common/utils -I$(top_srcdir)/src/database $(all_includes)
#METASOURCES = AUTO
#lib_LIBRARIES =  libform.a
#libform_a_SOURCES = FlightFormImpl.cpp FlightFormImpl.h GliderFormImpl.cpp \
#       GliderFormImpl.h GliderForm.ui MDIWindow.cpp MDIWindow.h TableWindow.cpp TableWindow.h \
#       WayPointFormImpl.cpp WayPointFormImpl.h WayPointForm.ui FlightForm.ui IWayPointForm.cpp \
#       IGliderForm.cpp IFlightForm.cpp RouteForm.ui RouteFormImpl.cpp IRouteForm.cpp \
#       ServicingForm.ui ServicingFormImpl.cpp IServicingForm.cpp ProgressDlg.cpp
#noinst_HEADERS = IWayPointForm.h IGliderForm.h IFlightForm.h RouteFormImpl.h \
#       IRouteForm.h ServicingFormImpl.h IServicingForm.h ProgressDlg.h
