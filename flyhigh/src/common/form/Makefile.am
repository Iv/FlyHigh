INCLUDES = -I$(top_srcdir)/src/common/container -I$(top_srcdir)/src/configuration/flytec -I$(top_srcdir)/src/configuration/interface -I$(top_srcdir)/src/database -I$(top_srcdir)/src/device/flytec -I$(top_srcdir)/src/track $(all_includes)
METASOURCES = AUTO
lib_LIBRARIES =  libform.a
libform_a_SOURCES = FlightFormImpl.cpp FlightFormImpl.h GliderFormImpl.cpp \
	GliderFormImpl.h GliderForm.ui MDIWindow.cpp MDIWindow.h TableWindow.cpp TableWindow.h \
	WayPointFormImpl.cpp WayPointFormImpl.h WayPointForm.ui FlightForm.ui IWayPointForm.cpp \
	IGliderForm.cpp IFlightForm.cpp RouteForm.ui RouteFormImpl.cpp IRouteForm.cpp \
	ServicingForm.ui ServicingFormImpl.cpp IServicingForm.cpp CTRForm.ui CTRFormImpl.cpp \
	ICTRForm.cpp
noinst_HEADERS = IWayPointForm.h IGliderForm.h IFlightForm.h RouteFormImpl.h \
	IRouteForm.h ServicingFormImpl.h IServicingForm.h CTRFormImpl.h ICTRForm.h
