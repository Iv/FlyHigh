include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/settings/database
                    ${CMAKE_SOURCE_DIR}/src/settings/device
                    ${QT_INCLUDES})

add_subdirectory(database)
add_subdirectory(flytec)
add_subdirectory(flytec6015)
add_subdirectory(device)

set(settings_SRCS DbSettingsDlg.cpp)

add_library(settings ${settings_SRCS})


