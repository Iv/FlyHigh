include(${QT_USE_FILE})

include_directories( ${CMAKE_SOURCE_DIR}/src/common/qjson4
                     ${QT_INCLUDES} )

set(upload_SRCS Account.cpp
                IOLCUploader.cpp
                OLCUploaderFactory.cpp
                XContestUploader.cpp )

set(upload_HDRS XContestUploader.h)

QT4_WRAP_CPP(upload_SRCS ${upload_HDRS})


add_library(upload ${upload_SRCS})
