include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/database
										${CMAKE_SOURCE_DIR}/src/database/sql
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
							ProgressDlg.cpp
							PreferencesDlg.cpp
							MigrationDlg.cpp
							MigratorThread.cpp
							DatabaseWidget.cpp)

set(form_HDRS FlightFormImpl.h
              GliderFormImpl.h
              MDIWindow.h
              TableWindow.h
              WayPointFormImpl.h
              RouteFormImpl.h
              ServicingFormImpl.h
							ProgressDlg.h
							PreferencesDlg.h
							MigrationDlg.h
							MigratorThread.h
							DatabaseWidget.h)

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

target_link_libraries(form dbsql)
