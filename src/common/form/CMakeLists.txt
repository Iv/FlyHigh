include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/database/sql
                    ${CMAKE_SOURCE_DIR}/src/common/form
                    ${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR})

set(form_SRCS AccountFormImpl.cpp
              AirSpaceFormImpl.cpp
              FlightFormImpl.cpp
              GliderFormImpl.cpp
              UploadFormImpl.cpp
              IAccountForm.cpp
              IAirSpaceForm.cpp
              IGliderForm.cpp
              IFlightForm.cpp
              IRouteForm.cpp
              IServicingForm.cpp
              IUploadForm.cpp
              IWayPointForm.cpp
              RouteFormImpl.cpp
              ServicingFormImpl.cpp
              WayPointFormImpl.cpp)

set(form_HDRS AccountFormImpl.h
              AirSpaceFormImpl.h
              FlightFormImpl.h
              GliderFormImpl.h
              RouteFormImpl.h
              ServicingFormImpl.h
              UploadFormImpl.h
              WayPointFormImpl.h)

set(form_UIS
  AccountForm.ui
	AirSpaceForm.ui
	FlightForm.ui
	GliderForm.ui
	RouteForm.ui
	ServicingForm.ui
  UploadForm.ui
	WayPointForm.ui)

QT4_WRAP_CPP(form_SRCS ${form_HDRS})

QT4_WRAP_UI(form_UI_HDRS ${form_UIS})

add_library(form ${form_SRCS} ${form_HDRS} ${form_UI_HDRS})

target_link_libraries(form dbsql upload)
