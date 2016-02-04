add_subdirectory(airspace)
add_subdirectory(flight)
add_subdirectory(lib)
add_subdirectory(route)
add_subdirectory(waypoint)

set(res_RC res.qrc)

qt5_add_resources(res_RC_SRCS ${res_RC})

add_library(res ${res_RC_SRCS})

add_dependencies(res
 lint_airspace
 lint_flight
 lint_lib
 lint_route
 lint_waypoint)

# install icon
install(FILES flyhigh.png DESTINATION ${FLYHIGH_ICON_DIR})
