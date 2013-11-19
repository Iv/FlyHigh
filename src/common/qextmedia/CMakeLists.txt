include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES} )

set(qextmedia_SRCS qexifimageheader.cpp)

add_library(qextmedia ${qextmedia_SRCS})
