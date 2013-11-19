include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/configuration/flytec
                    ${CMAKE_SOURCE_DIR}/src/configuration/interface
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${QT_INCLUDES} )

add_subdirectory(elevation)
add_subdirectory(form)
add_subdirectory(container)
add_subdirectory(utils)
add_subdirectory(map)
add_subdirectory(qextmedia)
add_subdirectory(qextserialport)
add_subdirectory(webmap)
add_subdirectory(window)
