include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES} )

set(utils_SRCS Error.cpp
               GnuPlot.cpp
               Tokenizer.cpp
               simplecrypt.cpp )

add_library(utils ${utils_SRCS})
