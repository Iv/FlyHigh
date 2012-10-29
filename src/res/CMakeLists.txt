include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES} )

add_subdirectory(flight)
add_subdirectory(route)
add_subdirectory(waypoint)

set(res_RC res.qrc)

qt4_add_resources(res_RC_SRCS ${res_RC})

add_library(res ${res_RC_SRCS})

add_dependencies(res
 lint_flight
 lint_route
 lint_waypoint)

# install icon
install(FILES flyhigh.png DESTINATION ${FLYHIGH_ICON_DIR})
