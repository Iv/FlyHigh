include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES} )

set(res_RC res.qrc)

qt4_add_resources( res_RC_SRCS ${res_RC} )

add_library(res ${res_RC_SRCS})

# install icon
install(FILES flyhigh.png DESTINATION ${FLYHIGH_ICON_DIR})
